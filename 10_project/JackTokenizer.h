#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class JackTokenizer {
public:
	JackTokenizer(string filename);
	bool JackTokenizer::hasMoreLines();
	void JackTokenizer::advance();
	string JackTokenizer::tokenType();
	string JackTokenizer::keyword();
	char JackTokenizer::symbol();
	string JackTokenizer::identifier();
	int JackTokenizer::intVal();
	string JackTokenizer::stringVal();
	string JackTokenizer::curToken();
	bool JackTokenizer::hasMoreToken();

	string line;
	string token;
	string cur_token;
	vector<string> line_tokens;
private:
	ifstream file;
	vector<string> keyword_token = {"class", "constructor", "function", "method",
		"field", "static", "var", "int", "char",
		"boolean", "void", "true", "false", "null",
		"this", "let", "do", "if", "else", "while", "return"};
	vector<char> symbol_token = { '{', '}', '(', ')', '[', ']', '.',
		',', ';', '+', '-', '*', '/', '&',
		'|', '<', '>', '=', '~'
	};
};

#endif