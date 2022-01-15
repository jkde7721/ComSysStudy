#include "codewriter.h"
#include <dirent.h>

using namespace std;

int main(int argc, char * argv[]) {
	string name = argv[1];

	int pos;
	vector<string> vmName;
	vmName.clear();
	string asmName;

	// file
	if ((pos = name.find(".vm")) != string::npos) {
		vmName.push_back(name);
		asmName = name.substr(0, pos) + ".asm";
	}
	// directory
	else {
		DIR *dir;
		struct dirent *diread;
		vector<char *> files;

		while ((diread = readdir(dir)) != nullptr)
			files.push_back(diread->d_name);
		closedir(dir);
		
		for (auto file : files)
			if (name.find(".vm") != string::npos)
				vmName.push_back(name);

		if ((pos = name.find_last_of("/")) != string::npos)
			asmName = name + "/" + name.substr(pos + 1) + ".asm";
		else
			asmName = name + "/" + name + ".asm";
	}

	codewriter* cw = new codewriter(asmName);
	parser* ps;
	for (int i = 0; i < vmName.size(); i++) {
		ps = new parser(vmName[i]);
		cw->setFileName(vmName[i]);
		do {
			ps->advance();
			if (ps->commandType() == C_ARITHMETIC)
				cw->writerArithmetic(ps->arg1());
			else if (ps->commandType == C_PUSH)
				cw->writePushPop(C_PUSH, ps->arg1(), ps->arg2());
			else if (ps->commandType == C_POP)
				cw->writePushPop(C_POP, ps->arg1(), ps->arg2());
		} while (ps->hasMoreCommand());
	}
}

// whole letter is lower.