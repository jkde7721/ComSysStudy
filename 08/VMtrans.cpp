#include <iostream>
#include "CodeWriter.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	string path = argv[1];
	if (path.substr(path.length() - 3).compare(".vm") == 0) {//file
		CodeWriter cw(path.substr(0, path.length()-3));
		cw.setFileName(argv[1]);
	}
	else {//directory
		CodeWriter cw(path + path.substr(path.rfind("\\")));
		vector<string> names;
		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.path().extension() == ".vm") {
				cw.setFileName(entry.path());
			}
		}
	}
}