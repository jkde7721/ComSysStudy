#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class CodeWriter {
public:
	CodeWriter(string f_name);
	void CodeWriter::setFileName();
	void CodeWriter::writerArithmetic(string command);
	void CodeWriter::WritePushPop(string command, string segment, int index);
	void CodeWriter::Close();
	void CodeWriter::stk_push(int x);
	void CodeWriter::stk_push(string x);
	void CodeWriter::stk_pop();
	void CodeWriter::decSP();
	void CodeWriter::incSP();
	void CodeWriter::op(string command);

	void CodeWriter::writerInit();
	void CodeWriter::writeLabel(string label);
	void CodeWriter::writeGoto(string label);
	void CodeWriter::writeIf(string label);
	void CodeWriter::writeCall(string functionName, int numArgs);
	void CodeWriter::writeReturn();
	void CodeWriter::writeFunction(string functionName, int numLocals);

	string filename;
	int eqNum = 0;
	int gtNum = 0;
	int ltNum = 0;
	int returnNum = 0;

	string function_name;
private:
	ofstream fout;
};
#endif