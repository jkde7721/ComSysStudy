#ifndef __COMPILATIONENGINE_H__
#define __COMPILATIONENGINE_H__

#include "JackTokenizer.h"
#include "VMWriter.h"
#include "SymbolTable.h"

using namespace std;

char op[] = { '+', '-', '*', '/', '&', '|', '<', '>', '=' };

class CompilationEngine {
public:
	JackTokenizer* jt;
	VMWriter* vmw;
	SymbolTable* st;
	string class_name;
	int while_index, if_index;

	CompilationEngine(string, string);
	void compileClass(void);
	void compileClassVarDec(void);
	void compileSubroutine(void);
	void compileParameterList(void);
	int compileVarDec(void);
	void compileStatements(void);
	void compileDo(void);
	void compileLet(void);
	void compileWhile(void);
	void compileReturn(void);
	void compileIf(void);
	void compileExpression(void);
	void compileTerm(void);
	int compileExpressionList(void);

	string getIdentifier(void);
	keyword getKeyword(void);
	char getSymbol(void);
	int getIntVal(void);
	string getStrVal(void);
	string getType(void);
};

#endif