#ifndef __CODE_H__
#define __CODE_H__

#include <map>
#include <string>

using namespace std;

class code {
private:
	map<string, string> dest;
	map<string, string> comp;
	map<string, string> jump;

public:
	code(void);
	string getDest(string);
	string getComp(string);
	string getJump(string);
};

#endif