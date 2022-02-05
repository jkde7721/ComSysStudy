#include "Parser.h"
#include <vector>

class CodeWriter {
private:
	ofstream outputStream;
	int eqCnt = 0;
	int gtCnt = 0;
	int ltCnt = 0;
	int reCnt = 0;
	string fname;
	vector<string> functionNames = {""};

public:
	CodeWriter(string path);
	void setFileName(filesystem::path fpath);
	void writerInit();
	void writeArithmetic(string command);
	void writePushPop(type command, string segment, int index);
	void writeLabel(string label);
	void writeGoto(string label);
	void writeIf(string label);
	void writeCall(string functionName, int numArgs);
	void writeReturn();
	void writeFunction(string functionName, int numLocals);
	void Close();

	void stackPush();
};