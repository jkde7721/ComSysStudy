#include "CompilationEngine.h"

CompilationEngine::CompilationEngine(string jackName, string xmlName){
	tab = "";
	jt = new JackTokenizer(jackName);
	writeFile.open(xmlName);

	compileClass();

	writeFile.close();
	delete jt;
}

void CompilationEngine::compileClass(void) {
	writeFile << "<class>" << endl;
	tab += "  ";

	writeKeyword(); // 'class'
	writeIdentifier(); // className - identifier
	writeSymbol(); // '{'

	// classVarDec*
	// enum keyword::STATIC = 9, FIELD = 10
	while (jt->tokenType() == KEYWORD && (jt->keyword() == 9 || jt->keyword() == 10))
		compileClassVarDec();

	// subroutineDec*
	// enum keyword::CONSTRUCTOR = 3, FUNCTION = 2, METHOD = 1
	while (jt->tokenType() == KEYWORD && (jt->keyword() == 3 || jt->keyword() == 2 || jt->keyword() == 1))
		compileSubroutine();

	writeSymbol(); // '}'

	tab = "";
	writeFile << "</class>" << endl;
}

void CompilationEngine::compileClassVarDec(void){
	writeFile << tab << "<classVarDec>" << endl;
	tab += "  ";

	writeKeyword(); // ('static' | 'field')
	compileType(); // type
	writeIdentifier(); // varName - identifier

	// (',' varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		writeSymbol(); // ','
		writeIdentifier(); // varName - identifier
	}

	writeSymbol(); // ';'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</classVarDec>" << endl;
}

void CompilationEngine::compileSubroutine(void){
	writeFile << tab << "<subroutineDec>" << endl;
	tab += "  ";

	writeKeyword(); // ('consturctor' | 'function' | 'method')

	// ('void' | type)
	// enum keyword::VOID = 7
	if (jt->tokenType() == KEYWORD && jt->keyword() == 7)
		writeKeyword();
	else
		compileType();

	writeIdentifier(); // subroutineName - identifier
	writeSymbol(); // '('

	if (jt->tokenType() != SYMBOL)
		compileParameterList(); // parameterList - 0 or 1 time

	writeSymbol(); // ')'
	compileSubroutineBody(); // subroutineBody

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</subroutineDec>" << endl;
}

void CompilationEngine::compileParameterList(void){
	writeFile << tab << "<parameterList>" << endl;
	tab += "  ";

	compileType(); // type
	writeIdentifier(); // varName - identifier

	// (',' type varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		writeSymbol(); // ','
		compileType(); // type
		writeIdentifier(); // varName - identifier
	}

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</parameterList>" << endl;
}

void CompilationEngine::compileVarDec(void){
	writeFile << tab << "<varDec>" << endl;
	tab += "  ";

	writeKeyword(); // 'var'
	compileType(); // type
	writeIdentifier(); // varName - identifier

	// (',' varName)*
	while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
		writeSymbol(); // ','
		writeIdentifier(); // varName - identifier
	}

	writeSymbol(); // ';'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</varDec>" << endl;
}

void CompilationEngine::compileStatements(void){
	writeFile << tab << "<statements>" << endl;
	tab += "  ";

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

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</statements>" << endl;
}

void CompilationEngine::compileDo(void){
	writeFile << tab << "<doStatement>" << endl;
	tab += "  ";

	writeKeyword(); // 'do'

	// subroutineCall
	// ((className | varName) '.')? subroutineName '(' expressionList ')'
	writeIdentifier(); // (className | varName) | subroutineName - identifier
	if (jt->symbol() == '.') {
		writeSymbol(); // '.'
		writeIdentifier(); // subroutineName - identifier
	}
	writeSymbol(); // '('
	compileExpressionList(); // expressionList
	writeSymbol(); // ')'

	writeSymbol(); // ';'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</doStatement>" << endl;
}

void CompilationEngine::compileLet(void){
	writeFile << tab << "<letStatement>" << endl;
	tab += "  ";

	writeKeyword(); // 'let'
	writeIdentifier(); // varName - identifier

	// ('[' expression ']')?
	if (jt->tokenType() == SYMBOL && jt->symbol() == '[') {
		writeSymbol(); // '['
		compileExpression(); // expression
		writeSymbol(); // ']'
	}

	writeSymbol(); // '='
	compileExpression(); // expression
	writeSymbol(); // ';'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</letStatement>" << endl;
}

void CompilationEngine::compileWhile(void){
	writeFile << tab << "<whileStatement>" << endl;
	tab += "  ";

	writeKeyword(); // 'while'
	writeSymbol(); // '('
	compileExpression(); // expression
	writeSymbol(); // ')'
	writeSymbol(); // '{'
	compileStatements(); // statements
	writeSymbol(); // '}'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</whileStatement>" << endl;
}

void CompilationEngine::compileReturn(void){
	writeFile << tab << "<returnStatement>" << endl;
	tab += "  ";

	writeKeyword(); // 'return'

	// expression?
	if (jt->symbol() != ';')
		compileExpression();

	writeSymbol(); // ';'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</returnStatement>" << endl;
}

void CompilationEngine::compileIf(void){
	writeFile << tab << "<ifStatement>" << endl;
	tab += "  ";

	writeKeyword(); // 'if'
	writeSymbol(); // '('
	compileExpression(); // expression
	writeSymbol(); // ')'
	writeSymbol(); // '{'
	compileStatements(); // statements
	writeSymbol(); // '}'

	// ('else' '{' statements '}')?
	if (jt->tokenType() == KEYWORD && jt->keyword() == ELSE) {
		writeKeyword(); // 'else'
		writeSymbol(); // '{'
		compileStatements(); // statements
		writeSymbol(); // '}'
	}

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</ifStatement>" << endl;
}

void CompilationEngine::compileExpression(void){
	writeFile << tab << "<expression>" << endl;
	tab += "  ";

	compileTerm(); // term

	// (op term)*
	while (*find(begin(op), end(op), jt->symbol()) == jt->symbol()) {
		writeSymbol(); // op - '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
		compileTerm(); // term
	}

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</expression>" << endl;
}

void CompilationEngine::compileTerm(void){ // no tab +
	// integerConstant
	if (jt->tokenType() == INT_CONST)
		writeIntVal();

	// stringConstant
	else if (jt->tokenType() == STRING_CONST)
		writeStrVal();

	// keywordConstant - 'true' | 'false' | 'null' | 'this'
	// enum keyword::TRUE = 17, FALSE = 18, K_NULL = 19, THIS = 20
	else if (jt->tokenType() == KEYWORD && (jt->keyword() >= 17 && jt->keyword() <= 20))
		writeKeyword();

	// varName | varName '[' expression ']' | subroutineCall
	else if (jt->tokenType() == IDENTIFIER) {
		writeIdentifier(); // varName - identifier | subroutineCall - (className | varName) | subroutineName - identifier
		
		// varName '[' expression ']'
		if (jt->tokenType() == SYMBOL && jt->symbol() == '[') {
			writeSymbol(); // '['
			compileExpression(); // expression
			writeSymbol(); // ']'
		}

		// subroutineCall - subroutineName '(' expressionList ')'
		else if (jt->tokenType() == SYMBOL && jt->symbol() == '(') {
			writeSymbol(); // '('
			compileExpressionList(); // expressionList
			writeSymbol(); // ')'
		}
		// subroutineCall - (className | varName) '.' subroutineName '(' expressionList ')'
		else if (jt->tokenType() == SYMBOL && jt->symbol() == '.') {
			writeSymbol(); // '.'
			writeIdentifier(); // subroutineName - identifier
			writeSymbol(); // '('
			compileExpressionList(); // expressionList
			writeSymbol(); // ')'
		}
	}

	// '(' expression ')'
	else if (jt->tokenType() == SYMBOL && jt->symbol() == '(') {
		writeSymbol(); // '('
		compileExpression(); // expression
		writeSymbol(); // ')'
	}

	// unaryOp term
	else if (jt->tokenType() == SYMBOL && (jt->symbol() == '-' || jt->symbol() == '~')) {
		writeSymbol(); // unaryOp - '-' | '~'
		compileTerm();
	}
}

void CompilationEngine::compileExpressionList(void){
	writeFile << tab << "<expressionList>" << endl;
	tab += "  ";

	// (expression (',' expression)*)?
	if (jt->symbol() != ')') {
		compileExpression(); // expression

		// (',' expression)*
		while (jt->tokenType() == SYMBOL && jt->symbol() == ',') {
			writeSymbol(); // ','
			compileExpression(); // expression
		}
	}

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</expressionList>" << endl;
}


void CompilationEngine::writeKeyword(void){
	writeFile << tab << "<keyword> " << keyword_ele[jt->keyword()] << " </keyword>" << endl;
	jt->advance();
}

void CompilationEngine::writeSymbol(void){
	writeFile << tab << "<symbol> " << jt->symbol() << " </symbol>" << endl;
	if (jt->hasMoreTokens())
		jt->advance();
}

void CompilationEngine::writeIntVal(void){
	writeFile << tab << "<integerConstant> " << jt->intVal() << " </integerConstant>" << endl;
	jt->advance();
}

void CompilationEngine::writeStrVal(void){
	writeFile << tab << "<stringConstant> " << jt->stringVal() << " </stringConstant>" << endl;
	jt->advance();
}

void CompilationEngine::writeIdentifier(void){
	writeFile << tab << "<identifier> " << jt->identifier() << " </identifier>" << endl;
	jt->advance();
}

void CompilationEngine::compileType(void){
	// 'int' | 'char' | 'boolean' | className
	// enum keyword::INT = 4, CHAR = 6, BOOLEAN = 5
	if (jt->tokenType() == KEYWORD && (jt->keyword() == 4 || jt->keyword() == 6 || jt->keyword() == 5))
		writeKeyword();
	else
		writeIdentifier();
}

void CompilationEngine::compileSubroutineBody(void){
	writeFile << tab << "<subroutineBody>" << endl;
	tab += "  ";

	writeSymbol(); // '{'
				   
	// varDec*
	// enum keyword::VAR = 8
	while (jt->tokenType() == KEYWORD && jt->keyword() == 8)
		compileVarDec();

	compileStatements(); // statements
	writeSymbol(); // '}'

	tab.substr(0, tab.length() - 2);
	writeFile << tab << "</subroutineBody>" << endl;
}