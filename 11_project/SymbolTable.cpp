#include "SymbolTable.h"

SymbolTable::SymbolTable(void) {
	classTable.clear();
	static_index = 0;
	field_index = 0;

	subroutineTable.clear();
	arg_index = 0;
	var_index = 0;
}

void SymbolTable::startSubroutine(void) {
	map<string, Identifier*>::iterator iter;
	for (iter = subroutineTable.begin(); iter != subroutineTable.end(); iter++)
		delete iter->second;

	subroutineTable.clear();
	arg_index = 0;
	var_index = 0;
}

void SymbolTable::define(string name, string type, iden_kind kind) {
	switch (kind) {
	case STATIC:
		classTable.insert({ name, new Identifier(name, type, kind, static_index) });
		static_index++;
		break;
	case FIELD:
		classTable.insert({ name, new Identifier(name, type, kind, field_index) });
		field_index++;
		break;
	case ARG:
		subroutineTable.insert({ name, new Identifier(name, type, kind, arg_index) });
		arg_index++;
		break;
	case VAR:
		subroutineTable.insert({ name, new Identifier(name, type, kind, var_index) });
		var_index++;
	}
}

int SymbolTable::varCount(iden_kind kind) {
	switch (kind) {
	case STATIC:
		return static_index;
	case FIELD:
		return field_index;
	case ARG:
		return arg_index;
	case VAR:
		return var_index;
	}
}

iden_kind SymbolTable::kindOf(string name) {
	if (in_subroutine) {
		auto item = subroutineTable.find(name);
		if (item != subroutineTable.end())
			return subroutineTable[name]->kind;
	}
	auto item = classTable.find(name);
	if (item != classTable.end())
		return classTable[name]->kind;
	return NONE;
}

string SymbolTable::typeOf(string name) {
	if (in_subroutine) {
		auto item = subroutineTable.find(name);
		if (item != subroutineTable.end())
			return subroutineTable[name]->type;
	}
	auto item = classTable.find(name);
	return classTable[name]->type;
}

int SymbolTable::indexOf(string name) {
	if (in_subroutine) {
		auto item = subroutineTable.find(name);
		if (item != subroutineTable.end())
			return subroutineTable[name]->index;
	}
	auto item = classTable.find(name);
	return classTable[name]->index;
}