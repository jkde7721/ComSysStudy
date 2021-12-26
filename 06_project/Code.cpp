#include "Code.h"
using namespace std;

/*
테이블에서 해당하는 string을 찾고, 있으면 이진 기호 리턴
없으면 "" 리턴
*/

Code::Code() {
	compTable["0"] = "0101010";
	compTable["1"] = "0111111";
	compTable["-1"] = "0111010";
	compTable["D"] = "0001100";
	compTable["A"] = "0110000";
	compTable["!D"] = "0001101";
	compTable["!A"] = "0110001";
	compTable["-D"] = "0001111";
	compTable["-A"] = "0110011";
	compTable["D+1"] = "0011111";
	compTable["A+1"] = "0110111";
	compTable["D-1"] = "0001110";
	compTable["A-1"] = "0110010";
	compTable["D+A"] = "0000010";
	compTable["A+D"] = "0000010";
	compTable["D-A"] = "0010011";
	compTable["A-D"] = "0000111";
	compTable["D&A"] = "0000000";
	compTable["D|A"] = "0010101";
	compTable["M"] = "1110000";
	compTable["!M"] = "1110001";
	compTable["-M"] = "1110011";
	compTable["M+1"] = "1110111";
	compTable["M-1"] = "1110010";
	compTable["D+M"] = "1000010";
	compTable["D-M"] = "1010011";
	compTable["M-D"] = "1000111";
	compTable["D&M"] = "1000000";
	compTable["D|M"] = "1010101";

	destTable[""] = "000";
	destTable["M"] = "001";
	destTable["D"] = "010";
	destTable["MD"] = "011";
	destTable["A"] = "100";
	destTable["AM"] = "101";
	destTable["AD"] = "110";
	destTable["AMD"] = "111";

	jumpTable[""] = "000";
	jumpTable["JGT"] = "001";
	jumpTable["JEQ"] = "010";
	jumpTable["JGE"] = "011";
	jumpTable["JLT"] = "100";
	jumpTable["JNE"] = "101";
	jumpTable["JLE"] = "110";
	jumpTable["JMP"] = "111";
}

string Code::dest(string dest_str) {
	if (destTable.find(dest_str) != destTable.end()) {
		return destTable[dest_str];
	}
	return "";
}
string Code::comp(string comp_str) {
	if (compTable.find(comp_str) != compTable.end()) {
		return compTable[comp_str];
	}
	return "";
}
string Code::jump(string jump_str) {
	if (jumpTable.find(jump_str) != jumpTable.end()) {
		return jumpTable[jump_str];
	}
	return "";
}