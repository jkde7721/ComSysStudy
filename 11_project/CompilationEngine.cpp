#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(string jackName, string vmName) {
	jt = new JackTokenizer(jackName);
	vmw = new VMWriter(vmName);
	st = new SymbolTable();
	class_name.clear();
	while_index = 0;
	if_index = 0;
	st->in_subroutine = false;

	compileClass();

	delete jt;
	delete vmw;

	map<string, Identifier*>::iterator iter;
	for (iter = st->subroutineTable.begin(); iter != st->subroutineTable.end(); iter++)
		delete iter->second;
	for (iter = st->classTable.begin(); iter != st->classTable.end(); iter++)
		delete iter->second;
	delete st;
}

void CompilationEngine::compileClass(void) {
	jt->advance(); // 'class'
	class_name = getIdentifier(); // className - identifier
	jt->advance(); // '{'

	// classVarDec*
	while (jt->tokenType() == KEYWORD && (jt->keyword() == keyword::STATIC || jt->keyword() == keyword::FIELD))
		compileClassVarDec();

	// subroutineDec*
	while (jt->tokenType() == KEYWORD && (jt->keyword() == keyword::CONSTRUCTOR || jt->keyword() == keyword::FUNCTION || jt->keyword() == keyword::METHOD))
		compileSubroutine();
	st->in_subroutine = false;

	jt->advance(); // '}'
}

void CompilationEngine::compileClassVarDec(void) {
	keyword kind_key = getKeyword(); // ('static' | 'field')
	string type = getType(); // type
	string name = getIdentifier(); // varName - identifier

	iden_kind kind;
	if (kind_key == keyword::STATIC)
		kind = iden_kind::STATIC;
	else if (kind_key == keyword::FIELD)
		kind = iden_kind::FIELD;

	if (st->kindOf(name) == NONE)
		st->define(name, type, kind);

	// (',' varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		jt->advance(); // ','
		name=getIdentifier(); // varName - identifier

		if (st->kindOf(name) == NONE)
			st->define(name, type, kind);
	}

	jt->advance(); // ';'
}

void CompilationEngine::compileSubroutine(void) {
	st->in_subroutine = true;
	st->startSubroutine();
	
	keyword subroutine_type = getKeyword(); // ('constructor' | 'function' | 'method')

	string return_type;
	// ('void' | type)
	if (jt->tokenType() == KEYWORD && jt->keyword() == keyword::VOID) {
		jt->advance();
		return_type = "void";
	}
	else
		return_type = getType();

	string subroutine_name = getIdentifier(); // subroutineName - identifier
	
	jt->advance(); // '('

	if (jt->tokenType() != SYMBOL)
		compileParameterList(); // parameterList - 0 or 1 time

	jt->advance(); // ')'

	// subroutineBody from here
	jt->advance(); // '{'

	// varDec*
	int nLocals = 0;
	while (jt->tokenType() == KEYWORD && jt->keyword() == keyword::VAR)
		nLocals += compileVarDec();

	vmw->writeFunction(class_name + "." + subroutine_name, nLocals);

	if (subroutine_type == keyword::CONSTRUCTOR) {
		vmw->writePush(Segment::CONST, st->varCount(iden_kind::FIELD));
		vmw->writeCall("Memory.alloc", 1);
		vmw->writePop(Segment::POINTER, 0);
	}
	else if (subroutine_type == keyword::METHOD) {
		st->define("this", "int", ARG);
		vmw->writePush(Segment::ARG, 0);
		vmw->writePop(Segment::POINTER, 0);
	}

	compileStatements(); // statements
	jt->advance(); // '}'

	if (return_type == "void")
		vmw->writePush(Segment::CONST, 0);
	vmw->writeReturn();
}

void CompilationEngine::compileParameterList(void) {
	string var_type = getType(); // type
	string var_name = getIdentifier(); // varName - identifier
	st->define(var_name, var_type, iden_kind::ARG);

	// (',' type varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		jt->advance(); // ','
		var_type = getType(); // type
		var_name = getIdentifier(); // varName - identifier
		st->define(var_name, var_type, iden_kind::ARG);
	}
}

int CompilationEngine::compileVarDec(void) {
	int nLocals = 0;

	if(jt->tokenType()==KEYWORD&&jt->keyword()==keyword::VAR)
		jt->advance(); // 'var'
	string var_type = getType(); // type
	string var_name = getIdentifier(); // varName - identifier
	st->define(var_name, var_type, iden_kind::VAR);
	nLocals++;

	// (',' varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		jt->advance(); // ','
		var_name = getIdentifier(); // varName - identifier
		st->define(var_name, var_type, iden_kind::VAR);
		nLocals++;
	}

	jt->advance(); // ';'
	return 0;
}

void CompilationEngine::compileStatements(void) {
	// statement*
	// statement - letStatement | ifStatement | whileStatement | doStatement | returnStatement
	while (jt->tokenType() == KEYWORD) {
		if (jt->keyword() == LET)
			compileLet();
		else if (jt->keyword() == IF)
			compileIf();
		else if (jt->keyword() == WHILE)
			compileWhile();
		else if (jt->keyword() == DO)
			compileDo();
		else if (jt->keyword() == RETURN)
			compileReturn();
		else
			break;
	}
}

void CompilationEngine::compileDo(void) {
	int argNum = 0;
	string subroutineName = "", fullName;

	jt->advance(); // 'do'

	// subroutineCall
	// ((className | varName) '.')? subroutineName '(' expressionList ')'
	string name = getIdentifier(); // (className | varName) | subroutineName - identifier
	if (jt->symbol() == '.') {
		jt->advance(); // '.'
		subroutineName = getIdentifier(); // subroutineName - identifier
	}

	if (subroutineName.empty()) {
		vmw->writePush(Segment::POINTER, 0);
		argNum++;
		fullName = class_name + "." + name;
	}
	else {
		if (st->kindOf(name) == iden_kind::NONE)
			fullName = name + "." + subroutineName;
		else {
			fullName = st->typeOf(name) + "." + subroutineName;
			argNum++;
			switch (st->kindOf(name)){
			case STATIC:
				vmw->writePush(Segment::STATIC, st->indexOf(name));
				break;
			case FIELD:
				vmw->writePush(Segment::THIS, st->indexOf(name));
				break;
			case ARG:
				vmw->writePush(Segment::ARG, st->indexOf(name));
				break;
			case VAR:
				vmw->writePush(Segment::LOCAL, st->indexOf(name));
			}
		}
	}

	jt->advance(); // '('
	argNum += compileExpressionList(); // expressionList
	jt->advance(); // ')'

	vmw->writeCall(fullName, argNum);

	jt->advance(); // ';'

	vmw->writePop(Segment::TEMP, 0);
}

void CompilationEngine::compileLet(void) {
	jt->advance(); // 'let'
	string varName = getIdentifier(); // varName - identifier

	// ('[' expression ']')?
	bool arr = false;
	if (jt->tokenType() == SYMBOL && jt->symbol() == '[') {
		arr = true;

		switch (st->kindOf(varName)) {
		case STATIC:
			vmw->writePush(Segment::STATIC, st->indexOf(varName));
			break;
		case FIELD:
			vmw->writePush(Segment::THIS, st->indexOf(varName));
			break;
		case ARG:
			vmw->writePush(Segment::ARG, st->indexOf(varName));
			break;
		case VAR:
			vmw->writePush(Segment::LOCAL, st->indexOf(varName));
		default:
		}

		jt->advance(); // '['
		compileExpression(); // expression
		jt->advance(); // ']'

		vmw->writeArithmetic(Command::ADD);
		vmw->writePop(Segment::POINTER, 1);
	}

	jt->advance(); // '='
	compileExpression(); // expression

	if (arr)
		vmw->writePop(Segment::THAT, 0);
	else {
		switch (st->kindOf(varName)) {
		case STATIC:
			vmw->writePop(Segment::STATIC, st->indexOf(varName));
			break;
		case FIELD:
			vmw->writePop(Segment::THIS, st->indexOf(varName));
			break;
		case ARG:
			vmw->writePop(Segment::ARG, st->indexOf(varName));
			break;
		case VAR:
			vmw->writePop(Segment::LOCAL, st->indexOf(varName));
		default:
		}
	}

	jt->advance(); // ';'
}

void CompilationEngine::compileWhile(void) {
	string startLabel = "W" + while_index;
	string endLabel = "WE" + while_index++;

	vmw->writeLabel(startLabel);

	jt->advance(); // 'while'
	jt->advance(); // '('
	compileExpression(); // expression
	jt->advance(); // ')'

	vmw->writeArithmetic(Command::NOT);
	vmw->writeIf(endLabel);

	jt->advance(); // '{'
	compileStatements(); // statements
	jt->advance(); // '}'

	vmw->writeGoto(startLabel);
	vmw->tab.substr(0, vmw->tab.length() - 2);

	vmw->writeLabel(endLabel);
	vmw->tab.substr(0, vmw->tab.length() - 2);
}

void CompilationEngine::compileReturn(void) {
	jt->advance(); // 'return'

	// expression?
	if (jt->symbol() != ';')
		compileExpression();

	jt->advance(); // ';'
}

void CompilationEngine::compileIf(void) {
	string trueLabel = "IT" + if_index;
	string falseLabel = "IF" + if_index;
	string endLabel = "IE" + if_index++;

	jt->advance(); // 'if'
	jt->advance(); // '('
	compileExpression(); // expression
	jt->advance(); // ')'

	vmw->writeArithmetic(Command::NOT);
	vmw->writeIf(falseLabel);

	jt->advance(); // '{'
	vmw->writeLabel(trueLabel);
	compileStatements(); // statements
	vmw->writeGoto(endLabel);
	jt->advance(); // '}'
	vmw->tab.substr(0, vmw->tab.length() - 2);

	vmw->writeLabel(falseLabel);
	// ('else' '{' statements '}')?
	if (jt->tokenType() == KEYWORD && jt->keyword() == ELSE) {
		jt->advance(); // 'else'
		jt->advance(); // '{'
		compileStatements(); // statements
		jt->advance(); // '}'
	}
	vmw->tab.substr(0, vmw->tab.length() - 2);

	vmw->writeLabel(endLabel);
	vmw->tab.substr(0, vmw->tab.length() - 2);
}

void CompilationEngine::compileExpression(void) {
	compileTerm(); // term

	// (op term)*
	while (*find(begin(op), end(op), jt->symbol()) == jt->symbol()) {
		// op - '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
		Command command;
		auto iter = find(begin(op), end(op), jt->symbol());
		switch (distance(op, iter)) {
		case 0:
			command = Command::ADD;
			break;
		case 1:
			command = Command::SUB;
			break;
		case 2:
			command = Command::NEG;
			break;
		case 3:
			command = Command::EQ;
			break;
		case 4:
			command = Command::GT;
			break;
		case 5:
			command = Command::LT;
			break;
		case 6:
			command = Command::AND;
			break;
		case 7:
			command = Command::OR;
			break;
		case 8:
			command = Command::NOT;
		}
		compileTerm(); // term
		vmw->writeArithmetic(command);
	}
}

void CompilationEngine::compileTerm(void) {
	// integerConstant
	if (jt->tokenType() == INT_CONST)
		vmw->writePush(Segment::CONST, getIntVal());

	// stringConstant
	else if (jt->tokenType() == STRING_CONST) {
		string strVal = getStrVal();
		vmw->writePush(Segment::CONST, strVal.length());
		vmw->writeCall("String.new", 1);

		for (int i = 0; i < strVal.length(); i++) {
			vmw->writePush(Segment::CONST, strVal.at(i));
			vmw->writeCall("String.appendChar", 2);
		}
	}

	// keywordConstant - 'true' | 'false' | 'null' | 'this'
	// enum keyword::TRUE = 17, FALSE = 18, K_NULL = 19, THIS = 20
	else if (jt->tokenType() == KEYWORD && (jt->keyword() >= 17 && jt->keyword() <= 20)) {
		switch (jt->keyword()){
		case keyword::TRUE:
			vmw->writePush(Segment::CONST, 0);
			vmw->writeArithmetic(Command::NOT);
			break;
		case keyword::FALSE:
		case keyword::K_NULL:
			vmw->writePush(Segment::CONST, 0);
			break;
		case keyword::THIS:
			vmw->writePush(Segment::POINTER, 0);
		}
		jt->advance();
	}

	// varName | varName '[' expression ']' | subroutineCall
	else if (jt->tokenType() == IDENTIFIER) {
		string name = getIdentifier(); // varName - identifier | subroutineCall - (className | varName) | subroutineName - identifier
		
		switch (st->kindOf(name)) {
		case STATIC:
			vmw->writePush(Segment::STATIC, st->indexOf(name));
			break;
		case FIELD:
			vmw->writePush(Segment::THIS, st->indexOf(name));
			break;
		case ARG:
			vmw->writePush(Segment::ARG, st->indexOf(name));
			break;
		case VAR:
			vmw->writePush(Segment::LOCAL, st->indexOf(name));
		default:
		}

		// varName '[' expression ']'
		if (jt->tokenType() == SYMBOL && jt->symbol() == '[') {
			jt->advance(); // '['

			vmw->writePush(Segment::POINTER, 1);
			vmw->writePop(Segment::TEMP, 0);

			compileExpression(); // expression
			
			vmw->writeArithmetic(Command::ADD);
			vmw->writePop(Segment::POINTER, 1);
			vmw->writePush(Segment::THAT, 0);
			vmw->writePop(Segment::TEMP, 1);
			vmw->writePush(Segment::TEMP, 0);
			vmw->writePop(Segment::POINTER, 1);
			vmw->writePush(Segment::TEMP, 1);

			jt->advance(); // ']'
		}

		// subroutineCall - subroutineName '(' expressionList ')'
		else if (jt->tokenType() == SYMBOL && jt->symbol() == '(') {
			jt->advance(); // '('

			vmw->writePush(Segment::POINTER, 0);
			int argNum = compileExpressionList() + 1; // expressionList
			vmw->writeCall(class_name + "." + name, argNum);

			jt->advance(); // ')'
		}
		// subroutineCall - (className | varName) '.' subroutineName '(' expressionList ')'
		else if (jt->tokenType() == SYMBOL && jt->symbol() == '.') {
			jt->advance(); // '.'
			string subroutineName = getIdentifier(); // subroutineName - identifier
			
			string fullName;
			int argNum = 0;
			if (st->kindOf(name) == iden_kind::NONE)
				fullName = name + "." + subroutineName;
			else {
				fullName = st->typeOf(name) + "." + subroutineName;
				argNum++;
				switch (st->kindOf(name)) {
				case STATIC:
					vmw->writePush(Segment::STATIC, st->indexOf(name));
					break;
				case FIELD:
					vmw->writePush(Segment::THIS, st->indexOf(name));
					break;
				case ARG:
					vmw->writePush(Segment::ARG, st->indexOf(name));
					break;
				case VAR:
					vmw->writePush(Segment::LOCAL, st->indexOf(name));
				}
			}

			jt->advance(); // '('
			argNum += compileExpressionList(); // expressionList
			vmw->writeCall(fullName, argNum);
			jt->advance(); // ')'
		}
	}

	// '(' expression ')'
	else if (jt->tokenType() == SYMBOL && jt->symbol() == '(') {
		jt->advance(); // '('
		compileExpression(); // expression
		jt->advance(); // ')'
	}

	// unaryOp term
	else if (jt->tokenType() == SYMBOL && (jt->symbol() == '-' || jt->symbol() == '~')) {
		char symbol = getSymbol(); // unaryOp - '-' | '~'
		compileTerm();

		Command command;
		if (symbol == '-')
			command = Command::NEG;
		else
			command = Command::NOT;
		vmw->writeArithmetic(command);
	}
}

int CompilationEngine::compileExpressionList(void) {
	int num = 0;

	// (expression (',' expression)*)?
	if (jt->symbol() != ')') {
		compileExpression(); // expression
		num++;

		// (',' expression)*
		while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
			jt->advance(); // ','
			compileExpression(); // expression
			num++;
		}
	}
	return num;
}


string CompilationEngine::getIdentifier(void) {
	string iden = jt->identifier();
	jt->advance();
	return iden;
}

keyword CompilationEngine::getKeyword(void) {
	keyword kwd = jt->keyword();
	jt->advance();
	return kwd;
}

char CompilationEngine::getSymbol(void) {
	char symbol = jt->symbol();
	jt->advance();
	return symbol;
}

int CompilationEngine::getIntVal(void) {
	int intVal = jt->intVal();
	jt->advance();
	return intVal;
}

string CompilationEngine::getStrVal(void) {
	string strVal = jt->stringVal();
	jt->advance();
	return strVal;
}

string CompilationEngine::getType(void) {
	// 'int' | 'char' | 'boolean' | className
	// enum keyword::INT = 4, CHAR = 6, BOOLEAN = 5
	if (jt->tokenType() == KEYWORD && (jt->keyword() == 4 || jt->keyword() == 6 || jt->keyword() == 5)) {
		keyword kwd = getKeyword();
		switch (kwd) {
		case INT:
			return "int";
		case BOOLEAN:
			return "boolean";
		case CHAR:
			return "char";
		}
	}
	else
		return getIdentifier();
}