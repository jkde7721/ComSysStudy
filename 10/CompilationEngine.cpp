#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(filesystem::path fpath) {
	tokenizer = new JackTokenizer(fpath);
	//tokenizer->test();//토큰화 모듈 테스트
	
	filesystem::path outpath = fpath.parent_path();
	outpath += "\\test\\";
	outpath += fpath.stem().concat(".xml");
	outputStream.open(outpath);

	CompileClass();

	outputStream.close();
}

void CompilationEngine::getToken() {
	if (tokenizer->hasMoreTokens())
		tokenizer->advance();
}

void CompilationEngine::CompileClass() {
	outputStream << "<class>\n";

	getToken();
	keyword();// 'class'
	getToken();
	identifier();// className
	getToken();
	symbol();// '{'
	while (true) {
		getToken();
		if (tokenizer->tokenType() == KEYWORD) {
			switch (tokenizer->keyword()) {
			case STATIC:
			case FIELD:
				CompileClassVarDec();// classVarDec*
				break;
			case CONSTRUCTOR:
			case FUNCTION:
			case METHOD:
				CompileSubroutine();// subroutineDec*
				break;
			default: break;
			}
		}
		else break;
	}
	symbol();// '}'

	outputStream << " </class>";	
}

void CompilationEngine::CompileClassVarDec() {
	outputStream << "<classVarDec>\n";

	keyword();// 'static'|'field'
	//type
	getToken();
	if (tokenizer->tokenType() == KEYWORD) keyword();// 'int'|'char'|'boolean'
	else identifier();// className

	//varName (',' varName)*
	do {
		getToken();
		identifier();// varName
		getToken();
		if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == ';') break;
		symbol();// ','
	} while (true);
	symbol();// ';'

	outputStream << "</classVarDec>\n";
}

void CompilationEngine::CompileSubroutine() {
	outputStream << "<subroutineDec>\n";

	keyword();// 'constructor'|'function'|'method'
	// ('void'|type)
	getToken();
	if (tokenizer->tokenType() == KEYWORD) keyword();// 'void'|'int'|'char'|'boolean'
	else identifier();// className

	getToken();
	identifier();// subroutineName
	getToken();
	symbol();// '('
	getToken();
	compileParameterList();// parameterList
	symbol();// ')'

	// subroutineBody
	outputStream << "<subroutineBody>\n";
	getToken();
	symbol();// '{'
	// varDec*
	while (true) {
		getToken();
		if (tokenizer->tokenType() == KEYWORD && tokenizer->keyword() == VAR) {
			compileVarDec();
		}
		else break;
	}
	compileStatements();// statements
	symbol();// '}'
	outputStream << "</subroutineBody>\n";

	outputStream << "</subroutineDec>\n";
}

void CompilationEngine::compileParameterList() {
	outputStream << "<parameterList>\n";

	while (true) {
		if (tokenizer->tokenType() == SYMBOL) {
			if (tokenizer->symbol() == ')') {
				outputStream << "</parameterList>\n";
				return;
			}
			else if (tokenizer->symbol() == ',') {
				symbol();// ','
				getToken();
			}
		}
		// type
		if (tokenizer->tokenType() == KEYWORD) keyword();// 'int'|'char'|'boolean'
		else identifier();// className;
		// varName
		getToken();
		identifier();
		getToken();
	}
}

void CompilationEngine::compileVarDec() {
	outputStream << "<varDec>\n";

	keyword();// 'var'
	// type
	getToken();
	if (tokenizer->tokenType() == KEYWORD) keyword();// 'int'|'char'|'boolean'
	else identifier();// className;
	// varName
	getToken();
	identifier();
	while (true) {
		getToken();
		if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == ';') break;
		symbol();// ','
		getToken();
		identifier();// varName
	}
	symbol();// ';'

	outputStream << "</varDec>\n";
}

void CompilationEngine::compileStatements() {
	outputStream << "<statements>\n";

	// statement*
	while (true) {
		// statement
		switch (tokenizer->keyword()) {
		case LET:
			compileLet();
			getToken();
			break;
		case IF:
			compileIf();// if문의 경우 else 확인 때문에 이미 getToken 함
			break;
		case WHILE:
			compileWhile();
			getToken();
			break;
		case DO:
			compileDo();
			getToken();
			break;
		case RETURN:
			compileReturn();
			getToken();
			break;
		}
		if (tokenizer->tokenType() == SYMBOL) break;// statements 끝
	}

	outputStream << "</statements>\n";
}

void CompilationEngine::compileDo() {
	outputStream << "<doStatement>\n";

	keyword();// 'do'
	
	// subroutineCall
	getToken();
	identifier();// subroutineName|className|varName
	getToken();
	if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == '.') {// className|varName
		symbol();// '.'
		getToken();
		identifier();// subroutineName
		getToken();
	}
	symbol();// '('
	getToken();
	CompileExpressionList();// expressionList
	symbol();// ')'
	getToken();
	symbol();// ';'

	outputStream << "</doStatement>\n";
}

void CompilationEngine::compileLet() {
	outputStream << "<letStatement>\n";

	keyword();// 'let'
	getToken();
	identifier();// varName
	getToken();
	if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == '[') {
		symbol();// '['
		getToken();
		CompileExpression();// expression
		symbol();// ']'
		getToken();
	}
	symbol();// '='
	getToken();
	CompileExpression();// expression
	symbol();// ';'

	outputStream << "</letStatement>\n";
}

void CompilationEngine::compileWhile() {
	outputStream << "<whileStatement>\n";

	keyword();// 'while'
	// (expression)
	getToken();
	symbol();// '('
	getToken();
	CompileExpression();// expression
	symbol();// ')'

	// {statements}
	getToken();
	symbol();// '{'
	getToken();
	compileStatements();// statements
	symbol();// '}'

	outputStream << "</whileStatement>\n";
}

void CompilationEngine::compileReturn() {
	outputStream << "<returnStatement>\n";

	keyword();// 'return'
	// expression?
	getToken();
	if (tokenizer->tokenType() == SYMBOL) {
		symbol();// ';'
		outputStream << "</returnStatement>\n";
		return;
	}
	CompileExpression();// expression
	symbol();// ';'

	outputStream << "</returnStatement>\n";
}

void CompilationEngine::compileIf() {
	outputStream << "<ifStatement>\n";

	keyword();// 'if'
	// (expression)
	getToken();
	symbol();// '('
	getToken();
	CompileExpression();// expression
	symbol();// ')'

	// {statements}
	getToken();
	symbol();// '{'
	getToken();
	compileStatements();// statements
	symbol();// '}'

	// (else {statements})?
	getToken();
	if (tokenizer->tokenType() == KEYWORD && tokenizer->keyword() == ELSE) {
		keyword();// 'else'
		// {statements}
		getToken();
		symbol();// '{'
		getToken();
		compileStatements();// statements
		symbol();// '}'

		getToken();// statements 함수에서 다음 토큰 읽어들인 걸 맞춰주기 위함
	}

	outputStream << "</ifStatement>\n";
}

void CompilationEngine::CompileExpression() {
	outputStream << "<expression>\n";

	CompileTerm();// term
	while (true) {
		if (tokenizer->tokenType() == SYMBOL) {
			char tmp = tokenizer->symbol();
			if (tmp == '+' || tmp == '-' || tmp == '*' || tmp == '/' || tmp == '&' || tmp == '|' || tmp == '<' || tmp == '>' || tmp == '=') {
				symbol();// op
				getToken();
				CompileTerm();// term
			}
			else break;
		}
		else break;
	}
	
	
	outputStream << "</expression>\n";
}

void CompilationEngine::CompileTerm() {
	outputStream << "<term>\n";

	if (tokenizer->tokenType()==INT_CONST) {
		outputStream << "<integerConstant> " << tokenizer->intVal() << " </integerConstant>\n";// integerConstant
		getToken();
	}
	else if (tokenizer->tokenType()==STRING_CONST) {
		outputStream << "<stringConstant> " << tokenizer->stringVal() << " </stringConstant>\n";// stringConstant
		getToken();
	}
	else if (tokenizer->tokenType() == KEYWORD && (tokenizer->keyword() == TRUE || tokenizer->keyword() == FALSE || tokenizer->keyword() == KEY_NULL || tokenizer->keyword() == THIS)) {
		keyword();// keywordConstant
		getToken();
	}
	else if (tokenizer->tokenType() == IDENTIFIER) {
		identifier();// varName|className|subroutineName
		getToken();
		if (tokenizer->tokenType() == SYMBOL) {
			switch (tokenizer->symbol()) {
			case '[':// varName [expression]
				symbol();// '['
				getToken();
				CompileExpression();// expression
				symbol();// ']'
				getToken();
				break;
			case '.':// (className|varName).subroutineName(expressionList)
				symbol();// '.'
				getToken();
				identifier();// subroutineName
				getToken();
			case '('://subroutineName(expressionList)
				symbol();// '('
				getToken();
				CompileExpressionList();// expressionList
				symbol();// ')'
				getToken();
				break;
			}
		}
	}
	else if (tokenizer->tokenType() == SYMBOL) {
		symbol();// '('|unaryOP
		switch (tokenizer->symbol()) {
		case '(':// (expression)
			getToken();
			CompileExpression();
			symbol();// ')'
			getToken();
			break;
		default:// unaryOP term
			getToken();
			CompileTerm();// term
		}
	}

	outputStream << "</term>\n";
}

void CompilationEngine::CompileExpressionList() {
	outputStream << "<expressionList>\n";

	if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == ')') {
		outputStream << "</expressionList>\n";
		return;
	}
	while (true) {
		CompileExpression();// expression
		if (tokenizer->tokenType() == SYMBOL && tokenizer->symbol() == ')')
			break;
		symbol();// ','
		getToken();
	}

	outputStream << "</expressionList>\n";
}

void CompilationEngine::keyword() {
	switch (tokenizer->keyword()) {
	case CLASS:
		outputStream << "<keyword> class </keyword>\n";
		break;
	case METHOD:
		outputStream << "<keyword> method </keyword>\n";
		break;
	case FUNCTION:
		outputStream << "<keyword> function </keyword>\n";
		break;
	case CONSTRUCTOR:
		outputStream << "<keyword> constructor </keyword>\n";
		break;
	case INT:
		outputStream << "<keyword> int </keyword>\n";
		break;
	case BOOLEAN:
		outputStream << "<keyword> boolean </keyword>\n";
		break;
	case CHAR:
		outputStream << "<keyword> char </keyword>\n";
		break;
	case VOID:
		outputStream << "<keyword> void </keyword>\n";
		break;
	case VAR:
		outputStream << "<keyword> var </keyword>\n";
		break;
	case STATIC:
		outputStream << "<keyword> static </keyword>\n";
		break;
	case FIELD:
		outputStream << "<keyword> field </keyword>\n";
		break;
	case LET:
		outputStream << "<keyword> let </keyword>\n";
		break;
	case DO:
		outputStream << "<keyword> do </keyword>\n";
		break;
	case IF:
		outputStream << "<keyword> if </keyword>\n";
		break;
	case ELSE:
		outputStream << "<keyword> else </keyword>\n";
		break;
	case WHILE:
		outputStream << "<keyword> while </keyword>\n";
		break;
	case RETURN:
		outputStream << "<keyword> return </keyword>\n";
		break;
	case TRUE:
		outputStream << "<keyword> true </keyword>\n";
		break;
	case FALSE:
		outputStream << "<keyword> false </keyword>\n";
		break;
	case KEY_NULL:
		outputStream << "<keyword> null </keyword>\n";
		break;
	case THIS:
		outputStream << "<keyword> this </keyword>\n";
		break;

	}
}

void CompilationEngine::symbol() {
	char tmp = tokenizer->symbol();
	if (tmp == '<')
		outputStream << "<symbol> &lt; </symbol>\n";
	else if (tmp == '>')
		outputStream << "<symbol> &gt; </symbol>\n";
	else if (tmp == '"')
		outputStream << "<symbol> &quot; </symbol>\n";
	else if (tmp == '&')
		outputStream << "<symbol> &amp; </symbol>\n";
	else
		outputStream << "<symbol> " << tokenizer->symbol() << " </symbol>\n";
}

void CompilationEngine::integerConstant() {
	outputStream << "<integerConstant> " << tokenizer->intVal() << " </integerConstant>\n";
}

void CompilationEngine::stringConstant() {
	outputStream << "<stringConstant> " << tokenizer->stringVal() << " </stringConstant>\n";
}

void CompilationEngine::identifier() {
	outputStream << "<identifier> " << tokenizer->identifier() << " </identifier>\n";
}