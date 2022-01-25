#ifndef __CODEWRITER_H__
#define __CODEWRITER_H__

#include "parser.h"

class codewriter {
public:
	ofstream writeFile;
	string curFile;
	int asmLine;

	string curFuncName;
	int funcNum;
	
	codewriter(string);
	void setFileName(string);
	void writerArithmetic(string);
	void writePushPop(commandType, string, int);
	void close(void);

	void writerInit(void);
	void writeLabel(string);
	void writeGoto(string);
	void writeIf(string);
	void writeCall(string, int);
	void writeReturn(void);
	void writeFunction(string, int);

	void pushToStack(void);
	void popFromStack(void);
};

#endif