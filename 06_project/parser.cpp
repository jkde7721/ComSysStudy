#include "parser.h"

parser::parser(string fileName){
	lineCounter = 0;
	strFile.clear();

	// stream으로 읽어서 string으로 변환
	string strLine;
	ifstream fstream;
	fstream.open(fileName);
	while (!fstream.eof()) {
		getline(fstream, strLine);
		int pos;
		if ((pos = strLine.find("/")) != string::npos) {
			if (pos == 0)
				strLine.clear();
			else
				strLine = strLine.substr(0, pos);
		} // remove comments
		strLine.erase(remove_if(strLine.begin(), strLine.end(), [](unsigned char x) {return isspace(x); }), strLine.end()); // remove whitespace
		if(!strLine.empty())
			strFile.push_back(strLine);
	}
	fstream.close();
}

bool parser::hasMoreCommand(void){
	if (lineCounter != strFile.size())
		return true;
	return false;
}

void parser::advance(void){
	lineCounter++;
}

commandType parser::commandType(void){
	if (strFile[lineCounter].find("@") == 0)
		return A_COMMAND;
	else if (strFile[lineCounter].find("(") == 0)
		return L_COMMAND;
	return C_COMMAND;
}

string parser::symbol(void){
	if (strFile[lineCounter].find("@") == 0)
		return strFile[lineCounter].substr(1, strFile[lineCounter].size() - 1);
	else if (strFile[lineCounter].find("(") == 0)
		return strFile[lineCounter].substr(1, strFile[lineCounter].size() - 2);
}

string parser::dest(void){
	int pos;
	if ((pos = strFile[lineCounter].find("=")) != string::npos)
		return strFile[lineCounter].substr(0, pos);
	return "NULL";
}

string parser::comp(void){
	int startPos = 0, endPos = strFile[lineCounter].size();
	if (strFile[lineCounter].find("=") != string::npos)
		startPos = strFile[lineCounter].find("=")+1;
	if (strFile[lineCounter].find(";") != string::npos)
		endPos = strFile[lineCounter].find(";");
	return strFile[lineCounter].substr(startPos, endPos - startPos);
}

string parser::jump(void){
	int pos;
	if ((pos = strFile[lineCounter].find(";")) != string::npos)
		return strFile[lineCounter].substr(pos + 1, strFile[lineCounter].size() - pos - 1);
	return "NULL";
}

string parser::tenToTwo(int deci){
	string bin;
	while (deci != 0) {
		bin += (deci % 2 == 0 ? "0" : "1");
		deci /= 2;
	}
	reverse(bin.begin(), bin.end());
	return bin;
}

string parser::make16(string output){
	string result;
	result.clear();

	for (int i = 16 - output.size(); i > 0; i--)
		result += "0";
	result += output;
	return result;
}

bool parser::isNum(string num){
	for (int i = 0; i < num.size(); i++)
		if (!isdigit(num.at(i)))
			return false;
	return true;
}