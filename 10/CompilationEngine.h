#include "JackTokenizer.h"

class CompilationEngine {
private:
	JackTokenizer* tokenizer;
	ofstream outputStream;

public:
	CompilationEngine(filesystem::path fpath);//여기서 jacktokenizer 만들어서 쓰기
	void CompileClass();
	void CompileClassVarDec();
	void CompileSubroutine();
	void compileParameterList();
	void compileVarDec();
	void compileStatements();
	void compileDo();
	void compileLet();
	void compileWhile();
	void compileReturn();
	void compileIf();
	void CompileExpression();
	void CompileTerm();
	void CompileExpressionList();

	void getToken();

	void type();

	void keyword();
	void symbol();
	void integerConstant();
	void stringConstant();
	void identifier();

};