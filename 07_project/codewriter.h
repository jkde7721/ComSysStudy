#ifndef __CODEWRITER_H__
#define __CODEWRITER_H__

#include "parser.h"

class codewriter {
public:
	ofstream writeFile;
	string curFile;
	int asmLine;
	
	codewriter(string);
	void setFileName(string);
	void writerArithmetic(string);
	void writePushPop(commandType, string, int);
	void close(void);
};

#endif