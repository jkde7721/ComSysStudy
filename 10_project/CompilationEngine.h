#ifndef COMPILATIONENGINE_H
#define COMPILATIONENGINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class CompilationEngine {
public:
	CompilationEngine::CompilationEngine();
	void CompilationEngine::CompileClass();
	void CompilationEngine::CompileClassVarDec();
	void CompilationEngine::CompileSubroutine();
	void CompilationEngine::compileParameterList();
	void CompilationEngine::compileVarDec();
	void CompilationEngine::compileStatements();
	void CompilationEngine::compileDo();
	void CompilationEngine::compileLet();
	void CompilationEngine::compileWhile();
	void CompilationEngine::compileReturn();
	void CompilationEngine::compileIf();
	void CompilationEngine::CompileExpression();
	void CompilationEngine::CompileTerm();
	void CompilationEngine::CompileExpressionList();
private:
	ofstream fout;
};
#endif