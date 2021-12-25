#include "parser.h"
#include "code.h"
#include "symboltable.h"

int main(int argc, char * argv[]) {
	string fileName = "Prog.asm";
	string outFileName = fileName.substr(0, fileName.find("."));
	outFileName = outFileName + ".hack";

	parser* ps = new parser(fileName);
	code* cd = new code();
	symboltable* st = new symboltable();

	// 1 pass
	int counter = 0;

	while (ps->hasMoreCommand()) {
		if (ps->commandType() == L_COMMAND)
			st->addEntry(ps->symbol(), counter);
		else
			counter++;

		ps->advance();
	}

	// 2 pass
	ps->lineCounter = 0;
	int ramAdd = 16;

	ofstream outFile;
	outFile.open(outFileName);

	while (ps->hasMoreCommand()) {
		if (ps->commandType() == A_COMMAND) {
			string a_xxx;
			int int_xxx;

			a_xxx = ps->symbol();
			if (!(ps->isNum(a_xxx))) {
				if (!(st->contains(a_xxx))) {
					st->addEntry(a_xxx, ramAdd);
					ramAdd++;
				}
				int_xxx = st->getAddress(a_xxx);
			}
			else
				int_xxx = stoi(a_xxx);
			a_xxx = ps->tenToTwo(int_xxx);
			a_xxx = ps->make16(a_xxx);
			outFile << a_xxx << endl;
		}
		else if (ps->commandType() == C_COMMAND) {
			string dest = cd->getDest(ps->dest());
			string comp = cd->getComp(ps->comp());
			string jump = cd->getJump(ps->jump());
			outFile << "111" << comp << dest << jump << endl;
		}
		else {
			string l_xxx;
			int int_xxx;

			l_xxx = ps->symbol();
			int_xxx = st->getAddress(l_xxx);
			l_xxx = ps->tenToTwo(int_xxx);
			l_xxx = ps->make16(l_xxx);
			outFile << l_xxx << endl;
		}
		ps->advance();
	}

	outFile.close();
	return 0;
}