#include "Parser.h"
#include <cctype>
#include <iostream>

Parser::Parser(string fname) {//constructor
	inputStream.open(fname);
}

void Parser::Parsing() {
	advance();
	t = commandType();
	switch (t) {
	case SKIP:
		break;
	case C_ARITHMETIC:
		//cout << argv1 << "\n";
		break;
	case C_PUSH:
	case C_POP:
		argv1 = arg1();
		argv2 = arg2();
		//cout << argv1 << " " << argv2 << "\n";
		break;
	}
}

bool Parser::hasMoreCommands() {
	return !inputStream.eof();
}

void Parser::advance() {
	getline(inputStream, command);
	trimLeft(0);
}

type Parser::commandType() {
	string temp = "";
	int i;
	for (i = 0;i < command.length();i++) {
		if (isspace(command[i])) break;
		else temp += command[i];
	}
	trimLeft(i);
	if (temp.compare("") == 0 || temp.compare("//") == 0) return SKIP;
	else if (temp.compare("push") == 0) return C_PUSH;
	else if (temp.compare("pop") == 0) return C_POP;
	else {
		argv1 = temp;
		return C_ARITHMETIC;
	}
}

string Parser::arg1() {
	string arg = "";
	int i;
	for (i = 0;i < command.length();i++) {
		if (isspace(command[i])) break;
		else arg += command[i];
	}
	trimLeft(i);

	return arg;
}

int Parser::arg2() {
	string arg = "";
	int i;
	for (i = 0;i < command.length();i++) {
		if (isspace(command[i])) break;
		else arg += command[i];
	}
	trimLeft(i);

	return atoi(arg.c_str());
}

void Parser::Close() {
	inputStream.close();
}

void Parser::trimLeft(int pos) {
	for (int i = pos;i < command.length();i++) {
		if (!isspace(command[i])) {
			command = command.substr(i);
			break;
		}
	}
}

type Parser::getType() {
	return t;
}

string Parser::getArgv1() {
	return argv1;
}

int Parser::getArgv2() {
	return argv2;
}