#ifndef Parser_h
#include <fstream>
#include <string>
using namespace std;

enum type:int {
	C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, SKIP
};

class Parser {
private:
	ifstream inputStream;
	string command;
	type t;
	string argv1;
	int argv2;
public:
	Parser(string fname);
	bool hasMoreCommands();
	void advance();
	type commandType();
	string arg1();
	int arg2();

	void Parsing();
	void Close();
	void trimLeft(int pos);
	type getType();
	string getArgv1();
	int getArgv2();
};

#endif /* Parser_h */