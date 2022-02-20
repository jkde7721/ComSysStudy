#include <dirent.h>
#include "CompilationEngine.h"

int main(int argc, char * argv[]) {
	string name = argv[1];

	vector<string> jackName;
	jackName.clear();

	// file
	if (name.find(".jack") != string::npos)
		jackName.push_back(name);
	// directory
	else {
		DIR *dir;
		struct dirent *diread;
		vector<char *> files;

		while ((diread = readdir(dir)) != nullptr)
			files.push_back(diread->d_name);
		closedir(dir);

		for (auto file : files)
			if (name.find(".jack") != string::npos)
				jackName.push_back(name);
	}

	string vmName;
	for (int i = 0; i < jackName.size(); i++) {
		vmName = jackName[i].substr(0, jackName[i].find(".jack")) + ".vm";
		CompilationEngine ce(jackName[i], vmName);
	}
}
