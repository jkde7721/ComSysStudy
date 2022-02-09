#include "CompilationEngine.h"

using namespace std;

CompilationEngine::CompilationEngine(){}
void CompilationEngine::CompileClass() {
	fout << "<class>" << endl;

	writeKeyword();
	writeIdentifier();
	writeSymbol();

	while (jt.tokenType() == "KEYWORD" && (jt.keyword() == "static" || jt.keyword() == "field"))
		CompileClassVarDec();

	while (jt.tokenType() == "KEYWORD" && (jt.keyword() == "constructor" || jt.keyword() == "function" || jt.keyword() == "method"))
		CompileSubroutine();

	writeSymbol();
	fout << "</class>" << endl;
	fout << "</class>"<<endl;
}
void CompilationEngine::CompileClassVarDec() {
	fout << "<classVarDec>" << endl;

	if(jt.tokenType()=="KEYWORD")	writeKeyword();
	else if (jt.tokenType() == "IDENTIFIER")	writeIdentifier();

	writeIdentifier();
	while (jt.tokenType() == "SYMBOL"&&jt.cur_token == ',') {
		writeSymbol();
		writeIdentifier();
	}
	writeSymbol();

	fout << "</classVarDec>" << endl;
}
void CompilationEngine::CompileSubroutine() {
	fout << "<subroutineDec>" << endl;

	writeKeyword();

	if (jt.tokenType() == "KEYWORD")	writeKeyword();
	else if (jt.tokenType() == "IDENTIFIER")	writeIdentifier();

	writeIdentifier();

	writeSymbol();
	compileParameterList();
	writeSymbol();
	
	//compileSubroutineBody()
	fout << "<subroutineBody>" << endl;
	writeSymbol();
	while(jt.tokenType()=="KEYWORD"&&jt.cur_token=="var")	compileVarDec();
	compileStatements();
	writeSymbol();

	fout << "</subroutineBody>" << endl;
	fout << "/<subroutineDec>" << endl;
}
void CompilationEngine::compileParameterList() {
	fout << "<parameterList>" << endl;



	fout << "</parameterList>" << endl;
}
void CompilationEngine::compileVarDec() {}
void CompilationEngine::compileStatements() {}
void CompilationEngine::compileDo() {}
void CompilationEngine::compileLet() {}
void CompilationEngine::compileWhile() {}
void CompilationEngine::compileReturn() {}
void CompilationEngine::compileIf() {}
void CompilationEngine::CompileExpression() {}
void CompilationEngine::CompileTerm() {}
void CompilationEngine::CompileExpressionList() {}