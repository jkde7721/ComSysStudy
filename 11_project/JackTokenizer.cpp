#include "JackTokenizer.h"

JackTokenizer::JackTokenizer(string jackName) {
	tokenCounter = 0;
	tokens.clear();

	string line, token = "";
	int pos1, pos2;
	bool commentOn = false;
	ifstream fstream;
	fstream.open(jackName);
	while (!fstream.eof()) {
		getline(fstream, line);

		// remove comments - //
		if ((pos1 = line.find("//")) != string::npos) {
			if (pos1 == 0)
				line.clear();
			else
				line = line.substr(0, pos1);
		}
		// remove comments - /** */ & /* */
		if (commentOn == true) {
			if ((pos2 = line.find("*/")) != string::npos) { // ...*/... or */...
				line = line.substr(pos2 + 2);
				commentOn == false;
			}
			else
				line.clear();
		}
		if ((pos1 = line.find("/*")) != string::npos) {
			if ((pos2 = line.find("*/")) != string::npos) {
				if (pos1 == 0) // /*...*/... or /**...*/...
					line = line.substr(pos2 + 2);
				else // .../*...*/... or .../**...*/...
					line = line.substr(0, pos1) + line.substr(pos2 + 2);
			}
			else {
				if (pos1 == 0) // /*... or /**...
					line.clear();
				else // .../*... or .../**...
					line = line.substr(0, pos1);
				commentOn = true;
			}
		}

		// tokenizing
		while (!line.empty()) {
			// remove front whitespace
			while (isspace(line.front()))
				line = line.substr(1);

			if (line.empty())
				break;

			// keyword & identifier
			if (isalpha(line.front()) || line.front() == '_') {
				do {
					token += line.front();
					line = line.substr(1);
				} while (isalpha(line.front()) || isdigit(line.front()) || line.front() == '_');
				tokens.push_back(token);
				token.clear();
			}
			// symbol
			if (*find(begin(symbol_ele), end(symbol_ele), line.front()) == line.front()) {
				token += line.front();
				line = line.substr(1);
				tokens.push_back(token);
				token.clear();
			}
			// integerConstant
			while (isdigit(line.front())) {
				token += line.front();
				line = line.substr(1);
			}
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
			// StringConstant
			if (line.front() == '"') {
				token = line.substr(1);
				pos2 = token.find("\"");
				token = line.substr(0, pos2 + 2);
				tokens.push_back(token);
				token.clear();
			}
		}
	}
	fstream.close();
}

bool JackTokenizer::hasMoreTokens(void) {
	if (tokenCounter < tokens.size() - 1)
		return true;
	return false;
}

void JackTokenizer::advance(void) {
	tokenCounter++;
}

tokenType JackTokenizer::tokenType(void) {
	if (isdigit(tokens[tokenCounter].front()))
		return INT_CONST;

	if (tokens[tokenCounter].front() == '"')
		return STRING_CONST;

	string low_token = tokens[tokenCounter];
	transform(low_token.begin(), low_token.end(), low_token.begin(), [](unsigned char c) {return tolower(c); });
	if (*find(begin(keyword_ele), end(keyword_ele), low_token) == low_token)
		return KEYWORD;

	if (*find(begin(symbol_ele), end(symbol_ele), tokens[tokenCounter].front()) == tokens[tokenCounter].front())
		return SYMBOL;

	return IDENTIFIER;
}

keyword JackTokenizer::keyword(void) {
	string low_token = tokens[tokenCounter];
	transform(low_token.begin(), low_token.end(), low_token.begin(), [](unsigned char c) {return tolower(c); });

	if (low_token.find("class") != string::npos)
		return CLASS;
	else if (low_token.find("method") != string::npos)
		return METHOD;
	else if (low_token.find("function") != string::npos)
		return FUNCTION;
	else if (low_token.find("constructor") != string::npos)
		return CONSTRUCTOR;
	else if (low_token.find("int") != string::npos)
		return INT;
	else if (low_token.find("boolean") != string::npos)
		return BOOLEAN;
	else if (low_token.find("char") != string::npos)
		return CHAR;
	else if (low_token.find("void") != string::npos)
		return VOID;
	else if (low_token.find("var") != string::npos)
		return VAR;
	else if (low_token.find("static") != string::npos)
		return STATIC;
	else if (low_token.find("field") != string::npos)
		return FIELD;
	else if (low_token.find("let") != string::npos)
		return LET;
	else if (low_token.find("do") != string::npos)
		return DO;
	else if (low_token.find("if") != string::npos)
		return IF;
	else if (low_token.find("else") != string::npos)
		return ELSE;
	else if (low_token.find("while") != string::npos)
		return WHILE;
	else if (low_token.find("return") != string::npos)
		return RETURN;
	else if (low_token.find("true") != string::npos)
		return TRUE;
	else if (low_token.find("false") != string::npos)
		return FALSE;
	else if (low_token.find("null") != string::npos)
		return K_NULL;
	else if (low_token.find("this") != string::npos)
		return THIS;
}

char JackTokenizer::symbol(void) {
	return tokens[tokenCounter].front();
}

string JackTokenizer::identifier(void) {
	return tokens[tokenCounter];
}

int JackTokenizer::intVal(void) {
	return stoi(tokens[tokenCounter]);
}

string JackTokenizer::stringVal(void) {
	return tokens[tokenCounter].substr(1, tokens[tokenCounter].length() - 2);
}