#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <string>
#include <map>

using namespace std;

enum iden_kind {
	STATIC, FIELD, ARG, VAR, NONE
};

class Identifier {
public:
	string name;
	string type;
	iden_kind kind;
	int index;

	Identifier(string name, string type, iden_kind kind, int index) {
		this->name = name;
		this->type = type;
		this->kind = kind;
		this->index = index;
	}
};

class SymbolTable {
public:
	map<string, Identifier*> classTable;
	map<string, Identifier*> subroutineTable;
	int static_index, field_index, arg_index, var_index;
	bool in_subroutine;

	SymbolTable(void);
	void startSubroutine(void);
	void define(string, string, iden_kind);
	int varCount(iden_kind);
	iden_kind kindOf(string);
	string typeOf(string);
	int indexOf(string);
};

#endif