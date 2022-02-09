#include "JackTokenizer.h"
#include <cctype>
#include <set>;

set<string> keywordList = { "class", "constructor", "function", "method", "field", "static", "var",
	"int", "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else", "while", "return" };
set<string> symbolList = { "{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" };

JackTokenizer::JackTokenizer(filesystem::path fpath) {
	inputStream.open(fpath);

	outpath = fpath.parent_path();
	outpath += "\\test\\";
	outpath += fpath.stem().concat("T.xml");
	
	getline(inputStream, line);
}

void JackTokenizer::test() {
	ofstream toutputStream(outpath);

	toutputStream << "<tokens>\n";
	while (hasMoreTokens()) {
		advance();
		switch (tokenType()) {
		case KEYWORD:
			toutputStream << "<keyword> " << token << " </keyword>\n";
			break;
		case SYMBOL:
			if (token[0] == '<')
				toutputStream << "<symbol> &lt; </symbol>\n";
			else if (token[0] == '>')
				toutputStream << "<symbol> &gt; </symbol>\n";
			else if (token[0] == '"')
				toutputStream << "<symbol> &quot; </symbol>\n";
			else if (token[0] == '&')
				toutputStream << "<symbol> &amp; </symbol>\n";
			else
				toutputStream << "<symbol> " << token[0] << " </symbol>\n";
			break;
		case IDENTIFIER:
			toutputStream << "<identifier> " << token << " </identifier>\n";
			break;
		case INT_CONST:
			toutputStream << "<integerConstant> " << token << " </integerConstant>\n";
			break;
		case STRING_CONST:
			toutputStream << "<stringConstant> " << token << " </stringConstant>\n";
			break;
		}
	}
	toutputStream << "</tokens>";
	toutputStream.close();
}

bool JackTokenizer::hasMoreTokens() {
	while (true) {
		leftTrim();
		if (line.length() == 0) {
			if (!inputStream.eof()) getline(inputStream, line);
			else return false;
		}
		else if (line.length() > 1 && line[0] == '/') {
			if (line[1] != '/' && line[1] != '*') break; // symbol인 경우
			if (line[1] == '*') {
				while (line.find("*/") == string::npos) {
					getline(inputStream, line);
				}
			}
			if (!inputStream.eof()) {
				getline(inputStream, line);
			}
			else return false;
		}
		else break;
	}
	return true;
}

void JackTokenizer::advance() {//라인 앞에서 부터 토큰 읽고, 잘라내서 그 다음 토큰이 문장 맨 앞에 오도록 함
	int i;
	token = "";

	if (issymbol(0)) {
		token = line[0];
		line = line.substr(1);
		nowType = SYMBOL;
		return;
	}

	if (line[0] == '"') {//문자열 상수
		nowType = STRING_CONST;
		for (i = 1;i < line.length();i++) {
			if (line[i] == '"')
				break;
			token += line[i];
		}
		i++;
	}
	else {
		for (i = 0;i < line.length();i++) {
			if (issymbol(i))
				break;
			else if (isspace(line[i]) == 0)
				token += line[i];
			else
				break;
		}
	}

	line = line.substr(i);
	leftTrim();
}

bool JackTokenizer::issymbol(int idx) {
	string c = "";
	c = line[idx];
	if (symbolList.find(c) != symbolList.end()) return true;
	else return false;
}

tType JackTokenizer::tokenType() {
	if (keywordList.find(token) != keywordList.end()) {
		nowType = KEYWORD;
		return KEYWORD;
	}
	else if (symbolList.find(token) != symbolList.end()) {
		nowType = SYMBOL;
		return SYMBOL;
	}
	else if (isint()) {
		nowType = INT_CONST;
		return INT_CONST;
	}
	else if (nowType == STRING_CONST) {
		return STRING_CONST;
	}
	else {
		return IDENTIFIER;
	}
}

kw JackTokenizer::keyword() {
	if (token.compare("class") == 0)	return CLASS;
	else if (token.compare("method") == 0)	return METHOD;
	else if (token.compare("function") == 0)	return FUNCTION;
	else if (token.compare("constructor") == 0)	return CONSTRUCTOR;
	else if (token.compare("int") == 0)	return INT;
	else if (token.compare("boolean") == 0)	return BOOLEAN;
	else if (token.compare("char") == 0)	return CHAR;
	else if (token.compare("void") == 0)	return VOID;
	else if (token.compare("var") == 0)	return VAR;
	else if (token.compare("static") == 0)	return STATIC;
	else if (token.compare("field") == 0)	return FIELD;
	else if (token.compare("let") == 0)	return LET;
	else if (token.compare("do") == 0)	return DO;
	else if (token.compare("if") == 0)	return IF;
	else if (token.compare("else") == 0)	return ELSE;
	else if (token.compare("while") == 0)	return WHILE;
	else if (token.compare("return") == 0)	return RETURN;
	else if (token.compare("true") == 0)	return TRUE;
	else if (token.compare("false") == 0)	return FALSE;
	else if (token.compare("null") == 0)	return KEY_NULL;
	else if (token.compare("this") == 0)	return THIS;
}

char JackTokenizer::symbol() {
	if (token.compare("{") == 0)	return '{';
	else if (token.compare("}") == 0)	return '}';
	else if (token.compare("(") == 0)	return '(';
	else if (token.compare(")") == 0)	return ')';
	else if (token.compare("[") == 0)	return '[';
	else if (token.compare("]") == 0)	return ']';
	else if (token.compare(".") == 0)	return '.';
	else if (token.compare(",") == 0)	return ',';
	else if (token.compare(";") == 0)	return ';';
	else if (token.compare("+") == 0)	return '+';
	else if (token.compare("-") == 0)	return '-';
	else if (token.compare("*") == 0)	return '*';
	else if (token.compare("/") == 0)	return '/';
	else if (token.compare("&") == 0)	return '&';
	else if (token.compare("|") == 0)	return '|';
	else if (token.compare("<") == 0)	return '<';
	else if (token.compare(">") == 0)	return '>';
	else if (token.compare("=") == 0)	return '=';
	else if (token.compare("~") == 0)	return '~';
}

string JackTokenizer::identifier() {
	return token;
}

int JackTokenizer::intVal() {
	return atoi(token.c_str());
}

string JackTokenizer::stringVal() {
	return token.substr(1, token.length() - 2);
}

void JackTokenizer::leftTrim() {//line의 왼쪽 공백 제거
	int i = 0;
	for (;i < line.length();i++) {
		if (isspace(line[i]) == 0)
			break;
	}
	line = line.substr(i);
}

bool JackTokenizer::isint() {//토큰이 정수인지 확인
	return atoi(token.c_str()) != 0 || token.compare("0") == 0;
}