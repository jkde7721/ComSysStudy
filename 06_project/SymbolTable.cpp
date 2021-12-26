#include "SymbolTable.h"
using namespace std;

/*Code.h와 비슷하게 map 사용, 생성자로 미리 symboltable 만들어준 다음 사용*/
SymbolTable::SymbolTable() {
	/*책 125p 참고*/
	symboltable["SP"] = 0;
	symboltable["LCL"] = 1;
	symboltable["ARG"] = 2;
	symboltable["THIS"] = 3;
	symboltable["THAT"] = 4;
	symboltable["R0"] = 0;
	symboltable["R1"] = 1;
	symboltable["R2"] = 2;
	symboltable["R3"] = 3;
	symboltable["R4"] = 4;
	symboltable["R5"] = 5;
	symboltable["R6"] = 6;
	symboltable["R7"] = 7;
	symboltable["R8"] = 8;
	symboltable["R9"] = 9;
	symboltable["R10"] = 10;
	symboltable["R11"] = 11;
	symboltable["R12"] = 12;
	symboltable["R13"] = 13;
	symboltable["R14"] = 14;
	symboltable["R15"] = 15;
	symboltable["SCREEN"] = 16384;
	symboltable["KBD"] = 24576;
}

void SymbolTable::addEntry(string symbol, int address) {
	if (symboltable.find(symbol) == symboltable.end()) {
		symboltable[symbol] = address;
	}
}

bool SymbolTable::contains(string symbol){
	if (symboltable.find(symbol) != symboltable.end()) {
		return true;
	}
	else return false;
}

int SymbolTable::GetAddress(string symbol){
	if (symboltable.find(symbol) != symboltable.end()) {
		return symboltable[symbol];
	}
	return -1;
}