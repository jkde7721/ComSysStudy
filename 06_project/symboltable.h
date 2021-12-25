#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <map>
#include <string>

using namespace std;

class symboltable {
private:
	map<string, int> symbols;

public:
	symboltable(void);
	void addEntry(string, int);
	bool contains(string);
	int getAddress(string);
};

#endif