#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Parser.h"
#include "CodeWriter.h"

using namespace std;

int main() {
	/*
	string name;
	cin >> name;
	vector<string> paths;

	string f_name;

	if (name.find(".vm") == string::npos) {
		for (auto& p : std::experimental::filesystem::directory_iterator(name))
		{
			if(p.path().string().find(".vm")!=string::npos)
				paths.push_back(p.path().string());
		}

	}
	else {
		paths.push_back(name);
	}

	CodeWriter cw = CodeWriter(name);
	cw.setFileName();
	cw.writerInit();
	for (int i = 0; i < paths.size(); i++) {
		string f_name = paths[i];

		Parser ps = Parser(f_name);
		/*CodeWriter cw = CodeWriter(f_name);
		cw.setFileName();
		cw.writerInit();*//*

		while (ps.hasMoreCommands()) {
			if (ps.commandType() == "C_ARITHMETIC") {
				cw.writerArithmetic(ps.arg1());
			}
			else if (ps.commandType() == "C_PUSH")
				cw.WritePushPop("C_PUSH", ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_POP")
				cw.WritePushPop("C_POP", ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_LABEL")
				cw.writeLabel(ps.arg1());
			else if (ps.commandType() == "C_GOTO")
				cw.writeGoto(ps.arg1());
			else if (ps.commandType() == "C_IF")
				cw.writeIf(ps.arg1());
			else if (ps.commandType() == "C_FUNCTION")
				cw.writeFunction(ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_RETURN")
				cw.writeReturn();
			else if (ps.commandType() == "C_CALL")
				cw.writeCall(ps.arg1(), ps.arg2());
			ps.advance();
		}
		//cw.Close();

	}
	cw.Close();
	return 0;
	*/
	string f_name;
	cin >> f_name;

	Parser ps = Parser(f_name);
	CodeWriter cw = CodeWriter(f_name);
	cw.setFileName();
	//cw.writerInit();

	while (ps.hasMoreCommands()) {
		while (ps.hasMoreCommands()) {
			if (ps.commandType() == "C_ARITHMETIC") {
				cw.writerArithmetic(ps.arg1());
			}
			else if (ps.commandType() == "C_PUSH")
				cw.WritePushPop("C_PUSH", ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_POP")
				cw.WritePushPop("C_POP", ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_LABEL")
				cw.writeLabel(ps.arg1());
			else if (ps.commandType() == "C_GOTO")
				cw.writeGoto(ps.arg1());
			else if (ps.commandType() == "C_IF")
				cw.writeIf(ps.arg1());
			else if (ps.commandType() == "C_FUNCTION")
				cw.writeFunction(ps.arg1(), ps.arg2());
			else if (ps.commandType() == "C_RETURN")
				cw.writeReturn();
			else if (ps.commandType() == "C_CALL")
				cw.writeCall(ps.arg1(), ps.arg2());
			cout<<ps.commandType()<<"\n";
			ps.advance();
		}
		cw.Close();

		return 0;
	}
}