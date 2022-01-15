#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

enum commandType {
	C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL
};

class parser {
public:
	int lineCounter;
	vector<string> lines;

	parser(string);
	bool hasMoreCommand(void);
	void advance(void);
	commandType commandType(void);
	string arg1(void);
	int arg2(void);
};

#endif