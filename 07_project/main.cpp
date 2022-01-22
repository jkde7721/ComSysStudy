#include <iostream>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"

using namespace std;

int main() {
	string f_name;
	cin >> f_name;

	Parser ps = Parser(f_name);
	CodeWriter cw = CodeWriter(f_name);
	cw.setFileName();

	while (ps.hasMoreCommands()) {
		if (ps.commandType() == "C_ARITHMETIC") {
			cw.writerArithmetic(ps.arg1());
		}
		else if (ps.commandType() == "C_PUSH")
			cw.WritePushPop("C_PUSH", ps.arg1(), ps.arg2());
		else if (ps.commandType() == "C_POP")
			cw.WritePushPop("C_POP", ps.arg1(), ps.arg2());
		
		ps.advance();
	}
	cw.Close();

	return 0;
}