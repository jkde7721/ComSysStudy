#include "codewriter.h"

codewriter::codewriter(string asmName){
	writeFile.open(asmName);
	asmLine = 0;
}

void codewriter::setFileName(string filename){
	int pos = filename.find(".vm");
	curFile = filename.substr(0, pos);
	if ((pos = curFile.find_last_of("/")) != string::npos)
		curFile = curFile.substr(pos + 1);
} // for static

void codewriter::writerArithmetic(string command){
	if (command.compare("neg") == 0 || command.compare("not") == 0) {
		writePushPop(C_POP, "temp", 0);

		writeFile << "@R5" << endl;
		if (command.compare("neg") == 0)
			writeFile << "M=-M" << endl;
		else
			writeFile << "M=!M" << endl;
		asmLine += 2;
	}
	else {
		writePushPop(C_POP, "temp", 0);
		writePushPop(C_POP, "temp", 1);
		
		if (command.compare("eq") == 0 || command.compare("gt") == 0 || command.compare("lt") == 0) {
			writeFile << "@R5" << endl << "D=D-M" << endl; // x-y
			if (command.compare("eq") == 0)
				writeFile << "@EQ" << endl << "D;JEQ" << endl; // x-y=0
			else if (command.compare("gt") == 0)
				writeFile << "@GT" << endl << "D;JGT" << endl; // x-y>0
			else // lt
				writeFile << "@LT" << endl << "D;JLT" << endl; // x-y<0

			writeFile << "M=-1" << endl;
			asmLine += 5;
		}
		else {
			writeFile << "@R5" << endl;
			if (command.compare("add") == 0)
				writeFile << "D=D+M" << endl;
			else if (command.compare("sub") == 0)
				writeFile << "D=D-M" << endl;
			else if (command.compare("and") == 0)
				writeFile << "D=D&M" << endl;
			else // or
				writeFile << "D=D|M" << endl;
			writeFile << "M=D" << endl;
			asmLine += 3;
		}
	}
	writePushPop(C_PUSH, "temp", 0);
}

void codewriter::writePushPop(commandType command, string segment, int index){
	if (command == C_PUSH) {
		if (segment.compare("local") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 1 << endl;
			writeFile << "A=M+D" << endl;
			writeFile << "D=M" << endl;
			asmLine += 5;
		}
		else if (segment.compare("argument") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 2 << endl;
			writeFile << "A=M+D" << endl;
			writeFile << "D=M" << endl;
			asmLine += 5;
		}
		else if (segment.compare("this") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 3 << endl;
			writeFile << "A=M+D" << endl;
			writeFile << "D=M" << endl;
			asmLine += 5;
		}
		else if (segment.compare("that") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 4 << endl;
			writeFile << "A=M+D" << endl;
			writeFile << "D=M" << endl;
			asmLine += 5;
		}
		else if (segment.compare("pointer") == 0) {
			writeFile << "@" << 3 + index << endl;
			writeFile << "D=M" << endl;
			asmLine += 2;
		}
		else if (segment.compare("temp") == 0) {
			writeFile << "@" << 5 + index << endl;
			writeFile << "D=M" << endl;
			asmLine += 2;
		}
		else if (segment.compare("constant") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			asmLine += 2;
		}
		else { // static
			writeFile << "@" << curFile << "." << index << endl;
			writeFile << "D=M" << endl;
			asmLine += 2;
		}

		writeFile << "@SP" << endl << "A=M" << endl << "M=D" << endl;
		writeFile << "@SP" << endl << "M=M+1" << endl;
		asmLine += 5;
	}
	else { // command == C_POP
		// @sp a=m d=m, and to the place(@place), m=d
		// need to change whole content of below
		if (segment.compare("local") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 1 << endl;
			writeFile << "A=M+D" << endl;
			asmLine += 4;
		}
		else if (segment.compare("argument") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 2 << endl;
			writeFile << "A=M+D" << endl;
			asmLine += 4;
		}
		else if (segment.compare("this") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 3 << endl;
			writeFile << "A=M+D" << endl;
			asmLine += 4;
		}
		else if (segment.compare("that") == 0) {
			writeFile << "@" << index << endl;
			writeFile << "D=A" << endl;
			writeFile << "@" << 4 << endl;
			writeFile << "A=M+D" << endl;
			asmLine += 4;
		}
		else if (segment.compare("pointer") == 0) {
			writeFile << "@" << 3 + index << endl;
			asmLine++;
		}
		else if (segment.compare("temp") == 0) {
			writeFile << "@" << 5 + index << endl;
			asmLine++;
		}
		else if (segment.compare("constant") == 0) {
			writeFile << "@" << index << endl;
			asmLine++;
		}
		else { // static
			writeFile << "@" << curFile << "." << index << endl;
			asmLine++;
		}
		writeFile << "D=A" << endl << "@R13" << endl << "M=D";
		writeFile << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;
		writeFile << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		asmLine += 10;
	}
}

void codewriter::close(void){
	writeFile << "(END)" << endl << "@END" << endl << "0;JMP" << endl;

	writeFile << "(EQ)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;
	writeFile << "(GT)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;
	writeFile << "(LT)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;

	writeFile.close();
}