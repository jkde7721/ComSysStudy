#ifndef __COMPILATIONENGINE_H__
#define __COMPILATIONENGINE_H__

#include "JackTokenizer.h"

using namespace std;

char op[] = { '+', '-', '*', '/', '&', '|', '<', '>', '=' };

class CompilationEngine {
public:
	ofstream writeFile;
	string tab;
	JackTokenizer* jt;

	CompilationEngine(string, string);
	void compileClass(void);
	void compileClassVarDec(void);
	void compileSubroutine(void);
	void compileParameterList(void);
	void compileVarDec(void);
	void compileStatements(void);
	void compileDo(void);
	void compileLet(void);
	void compileWhile(void);
	void compileReturn(void);
	void compileIf(void);
	void compileExpression(void);
	void compileTerm(void);
	void compileExpressionList(void);

	void writeKeyword(void);
	void writeSymbol(void);
	void writeIntVal(void);
	void writeStrVal(void);
	void writeIdentifier(void);
	void compileType(void);
	void compileSubroutineBody(void);
};

#endif