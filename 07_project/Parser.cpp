#include "Parser.h"

using namespace std;

Parser::Parser(string filename) {
	file.open(filename);
}
bool Parser::hasMoreCommands() {
	return !file.eof();
}
void Parser::advance() {
	string line;
	if (hasMoreCommands()) {
		getline(file, line);
		if ((line.substr(0, 2) != "//") && (line.length() != 0)) {
			cur_command = line;
		}
	}
}
string Parser::commandType() {
	if (cur_command.length() == 0)	return "";
	else if (cur_command.find("push") != string::npos)	return "C_PUSH";
	else if (cur_command.find("pop") != string::npos)	return "C_POP";
	else if (cur_command.find("label") != string::npos)	return "C_LABEL";
	else if (cur_command.find("goto") != string::npos)	return "C_GOTO";
	else if (cur_command.find("if") != string::npos)	return "C_IF";
	else if (cur_command.find("function") != string::npos)	return "C_FUNCTION";
	else if (cur_command.find("return") != string::npos)	return "C_RETURN";
	else if (cur_command.find("call") != string::npos)	return "C_CALL";
	/*Q. 나머지는 다 산술연산으로 처리해도 되나?*/
	else return "C_ARITHMETIC";
}
string Parser::arg1() {
	command_type = commandType();
	if (command_type != "C_RETURN") {
		/*명령어 arg1 arg2라고 하면, 첫번째 space 다음에 arg1을 가져와서 리턴*/
		int first_space = cur_command.find(" ", 0);
		int second_space = cur_command.find(" ", first_space + 1);
		string arg1 = cur_command.substr(first_space+1, second_space-first_space-1);
		return arg1;
	}
}
int Parser::arg2() {
	command_type = commandType();
	if ((command_type == "C_PUSH")|| (command_type == "C_POP")|| (command_type == "C_FUNCTION")|| (command_type == "C_CALL")) {
		int first_space = cur_command.find(" ", 0);
		int second_space = cur_command.find(" ", first_space + 1);
		string arg2 = cur_command.substr(second_space + 1, cur_command.length());
		return stoi(arg2);
	}
}
/*
int main(void) {
	string filename;
	cin >> filename;
	Parser parse = Parser(filename);

	while (parse.hasMoreCommands()) {
		cout << "cur_command: " <<parse.cur_command<<'\n';
		cout << parse.arg1() << '\n';
		cout << parse.arg2() << '\n';
		cout << parse.command_type << '\n';
		parse.advance();
	}
	return 0;
}
*/