#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <map>
using namespace std;

class SymbolTable {
public:
	SymbolTable();
	bool SymbolTable::contains(string symbol);
	void SymbolTable::addEntry(string symbol, int address);
	int SymbolTable::GetAddress(string symbol);
private:
	map<string, int> symboltable;
};

#endif