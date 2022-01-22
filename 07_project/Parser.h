#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

class Parser {
public:
	Parser(string filename);
	bool Parser::hasMoreCommands();
	void Parser::advance();
	string Parser::commandType();
	string Parser::arg1();
	int Parser::arg2();

	string cur_command;
	string command_type;
private:
	ifstream file;
};
#endif