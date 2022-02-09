#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;


enum tType :int {
	KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
};
enum kw :int {
	CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, KEY_NULL, THIS
};

class JackTokenizer {
private:
	ifstream inputStream;
	filesystem::path outpath;
	tType nowType;
	

public:
	string token;
	string line;

	JackTokenizer(filesystem::path fpath);
	bool hasMoreTokens();
	void advance();
	tType tokenType();
	kw keyword();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();

	void leftTrim();
	bool issymbol(int idx);
	bool isint();
	bool isstring();

	void test();
};