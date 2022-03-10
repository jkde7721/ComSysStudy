#ifndef __VMWRITER_H__
#define __VMWRITER_H__

#include <string>
#include <fstream>

using namespace std;

enum Segment {
	CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP
};

string Segment_ele[] = {
	"constant", "argument", "local", "static", "this", "that", "pointer", "temp"
};

enum Command {
	ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT
};

string Command_ele[] = {
	"add","sub", "neg", "eq", "gt", "lt", "and", "or", "not"
};

class VMWriter {
public:
	ofstream writeFile;
	string tab;

	VMWriter(string);
	void writePush(Segment, int);
	void writePop(Segment, int);
	void writeArithmetic(Command);
	void writeLabel(string);
	void writeGoto(string);
	void writeIf(string);
	void writeCall(string, int);
	void writeFunction(string, int);
	void writeReturn(void);
	void close(void);
};

#endif
