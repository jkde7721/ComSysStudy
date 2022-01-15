#include "parser.h"

parser::parser(string vmName){
	lineCounter = -1;
	lines.clear();
	
	// stream으로 읽어서 string으로 변환
	string line;
	ifstream fstream;
	fstream.open(vmName);
	while (!fstream.eof()) {
		getline(fstream, line);
		int pos;
		if ((pos = line.find("/")) != string::npos) {
			if (pos == 0)
				line.clear();
			else
				line = line.substr(0, pos);
		} // remove comments
		line.erase(remove_if(line.begin(), line.end(), [](unsigned char x) {return isspace(x); }), line.end()); // remove whitespace
		if (!line.empty())
			lines.push_back(line);
	}
	fstream.close();
}

bool parser::hasMoreCommand(void){
	if (lineCounter < lines.size() - 1)
		return true;
	return false;
}

void parser::advance(void){
	lineCounter++;
}

commandType parser::commandType(void){
	if (lines[lineCounter].find("push") != string::npos)
		return C_PUSH;
	else if (lines[lineCounter].find("pop") != string::npos)
		return C_POP;
	else if (lines[lineCounter].find("label") != string::npos)
		return C_LABEL;
	else if (lines[lineCounter].find("goto") != string::npos)
		return C_GOTO;
	else if (lines[lineCounter].find("if-goto") != string::npos)
		return C_IF;
	else if (lines[lineCounter].find("function") != string::npos)
		return C_FUNCTION;
	else if (lines[lineCounter].find("return") != string::npos)
		return C_RETURN;
	else if (lines[lineCounter].find("call") != string::npos)
		return C_CALL;
	return C_ARITHMETIC;
}

string parser::arg1(void){
	int type = commandType();

	if (type == C_ARITHMETIC)
		return lines[lineCounter];

	int pos = lines[lineCounter].find(" ");
	string first = lines[lineCounter].substr(pos + 1);
	if (type == C_PUSH || type == C_POP || type == C_FUNCTION || type == C_CALL) {
		pos = first.find(" ");
		return first.substr(0, pos);
	}
	return first;
}

int parser::arg2(void){
	int pos = lines[lineCounter].find(" ");
	string second = lines[lineCounter].substr(pos + 1);
	pos = second.find(" ");
	second.substr(pos + 1);
	return stoi(second);
}