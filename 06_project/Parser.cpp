#include "Parser.h"

using namespace std;
/*
	각각의 함수 이름 및 기능은 책+a
	in: file을 읽어들여서 어셈블리 명령을 필드와 기호로 분해함
	out: 1. 공백과 주석 제거
		 2. Parser의 멤버에 각각의 분해한 명령 저장 -> dest, jump, comp, 
*/
Parser::Parser() {
	//파일을 open하고 읽기 시작함
	// .asm 확장자인지 확인하고 아니면 다시 input 받기 -> fileopen
	string file_name;
	cin >> file_name;
	///문자열 위치 검색 rfind 이용, 확장자 찾기
	while (file_name.rfind(".asm") == string::npos) {
		cout << "올바른 파일명이 아님. 다시 입력하세요\n";
		cin >> file_name;
	}

	filename = file_name;
	file.open(file_name);
	
}
bool Parser::hasMoreCommands() {
	//eof이면 false, eof가 아니면 true
	return !file.eof();
}
string Parser::advance() {
	//hasMoreCommands가 참이면
	//다음 줄로 넘어감
	//현재 해석하고 있는 line 변수에 임시로 담아둠
	//단, line에서 주석과 공백을 제거함
	string line;
	if(hasMoreCommands()){
		getline(file, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		//cout << line<<'\n';
		return line;
	}
}

char Parser::commandType(string line) {
	//"//"로 시작하는 경우, 주석처리(X)
	unsigned long commentPos = line.find("//");
	if (commentPos != string::npos) {
		line.erase(commentPos, line.length() - commentPos);
		command_type = 'X';
		return command_type;
	}
	currentLine = line;
	//line[0]이 "@"인 경우, A명령어
	if (line.front() == '@') {
		command_type = 'A';
		return command_type;

	}
	//line[0]이 "("인 경우, L명령어
	else if (line.front() == '(') {
		command_type = 'L';
		return command_type;
	}
	//else, C명령어
	else
	{
		command_type = 'C';
		return command_type;
	}
}

string Parser::symbol() {
	//A일 때 @빼기
	if (command_type == 'A') {
		return currentLine.substr(1, currentLine.length() - 1);
	}
	//L일 때 ()빼기
	unsigned long leftPos = currentLine.rfind('(');
	unsigned long rightPos = currentLine.find(')');
	if (leftPos != string::npos && rightPos != string::npos && command_type == 'L') {
		return currentLine.substr(leftPos + 1, rightPos - leftPos - 1);
	}
	return "";
}
/*
dest=cmp, dest=comp;jump, comp;jump의 경우 존재
;있으면 jump 있음
=있으면 dest 있음
*/
string Parser::dest() {
	if (command_type == 'C') {
		unsigned long equalPos;
		equalPos = currentLine.find("=");
		if (equalPos != string::npos) {
			return currentLine.substr(0, equalPos);
		}
	}
	return "";
}
string Parser::comp() {
	if (command_type == 'C') {
		unsigned long equalPos;
		unsigned long semicPos;
		
		equalPos = currentLine.find('=');
		semicPos = currentLine.find(';');
		
		if (equalPos != string::npos) {
			if (semicPos != string::npos) {
				return currentLine.substr(equalPos + 1, semicPos - equalPos - 1);
			}
			else {
				return currentLine.substr(equalPos + 1, currentLine.length() - equalPos - 1);
			}
		}
		else if (semicPos != string::npos) {
			return currentLine.substr(0, semicPos);
		}
	}
	return "";
}
string Parser::jump() {
	if (command_type == 'C') {
		unsigned long semicPos;
		semicPos = currentLine.find(';');
		if (semicPos != string::npos) {
			return currentLine.substr(semicPos + 1, currentLine.length() - semicPos - 1);
		}
	}
	return "";
}
/*
int main() {
	Parser parse = Parser();
	while (parse.hasMoreCommands()) {
		string lines = parse.advance();
		
		cout<<parse.commandType(lines)<<'\n';
		cout << parse.currentLine << '\n';
		cout << parse.symbol() << '\n';
		cout << parse.dest() << '\n';
		cout << parse.comp() << '\n';
		cout << parse.jump() << '\n';
	}
}
*/