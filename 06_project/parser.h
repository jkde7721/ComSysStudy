#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

enum commandType {
	A_COMMAND, C_COMMAND, L_COMMAND
};

class parser {
public:
	int lineCounter;
	vector<string> strFile;
	
	parser(string);
	bool hasMoreCommand(void);
	void advance(void);
	commandType commandType(void);
	string symbol(void);
	string dest(void);
	string comp(void);
	string jump(void);
	string tenToTwo(int);
	string make16(string);
	bool isNum(string);
};

#endif