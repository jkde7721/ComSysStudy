#include "Parser.h"

class CodeWriter {
private:
	ofstream outputStream;
	int eqCnt = 0;
	int gtCnt = 0;
	int ltCnt = 0;
public:
	CodeWriter();
	void setFileName(string fname);
	void writeArithmetic(string command);
	void writePushPop(type command, string segment, int index);
	void Close();
};