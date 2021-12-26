#ifndef CODE_H
#define CODE_H

#include <iostream>
#include <map>

using namespace std;

class Code {
public:
	Code();
	string Code::dest(string dest_str);
	string Code::comp(string dest_str);
	string Code::jump(string dest_str);

private:
	map<string, string> destTable;
	map<string, string> compTable;
	map<string, string> jumpTable;
};

#endif