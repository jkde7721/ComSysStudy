#include "symboltable.h"

symboltable::symboltable(void){
	symbols.insert({ "SP", 0 });
	symbols.insert({ "LCL", 1 });
	symbols.insert({ "ARG", 2 });
	symbols.insert({ "THIS", 3 });
	symbols.insert({ "THAT", 4 });
	symbols.insert({ "R0", 0 });
	symbols.insert({ "R1", 1 });
	symbols.insert({ "R2", 2 });
	symbols.insert({ "R3", 3 });
	symbols.insert({ "R4", 4 });
	symbols.insert({ "R5", 5 });
	symbols.insert({ "R6", 6 });
	symbols.insert({ "R7", 7 });
	symbols.insert({ "R8", 8 });
	symbols.insert({ "R9", 9 });
	symbols.insert({ "R10", 10 });
	symbols.insert({ "R11", 11 });
	symbols.insert({ "R12", 12 });
	symbols.insert({ "R13", 13 });
	symbols.insert({ "R14", 14 });
	symbols.insert({ "R15", 15 });
	symbols.insert({ "SCREEN", 16384 });
	symbols.insert({ "KBD", 24576 });
}

void symboltable::addEntry(string symbol, int address){
	symbols.insert({ symbol, address });
}

bool symboltable::contains(string symbol){
	if (symbols.find(symbol) != symbols.end())
		return true;
	return false;
}

int symboltable::getAddress(string symbol){
	return symbols[symbol];
}