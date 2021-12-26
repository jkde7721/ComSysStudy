#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

class Parser {
public:
	Parser();
	bool hasMoreCommands();
	string advance();
	char Parser::commandType(string line);
	string Parser::symbol();
	string Parser::dest();
	string Parser::comp();
	string Parser::jump();

	string filename;
	string currentLine;
	char command_type;
private:
	ifstream file;
	//string currentLine;
	//char command_type;
};

#endif