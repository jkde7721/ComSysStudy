#ifndef __JACKTOKENIZER_H__
#define __JACKTOKENIZER_H__

#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

enum tokenType {
	KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
};

enum keyword {
	CLASS, METHOD, FUNCTION, CONSTRUCTOR, 
	INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, 
	IF, ELSE, WHILE, RETURN, TRUE, FALSE, K_NULL, THIS
};

string keyword_ele[] = { 
	"class", "method", "function", "constructor", 
	"int", "boolean", "char", "void", "var", "static", "field", "let", "do", 
	"if", "else", "while", "return", "true", "false", "null", "this" 
};

char symbol_ele[] = { 
	'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' 
};

class JackTokenizer {
public:
	int tokenCounter;
	vector<string> tokens;

	JackTokenizer(string);
	bool hasMoreTokens(void);
	void advance(void);
	tokenType tokenType(void);
	keyword keyword(void);
	char symbol(void);
	string identifier(void);
	int intVal(void);
	string stringVal(void);
};

#endif