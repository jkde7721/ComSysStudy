#include "codewriter.h"

codewriter::codewriter(string asmName){
	writeFile.open(asmName);
	asmLine = 0;

	curFuncName.clear();
	funcNum = 0;
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
		pushToStack();
	}
	else { // command == C_POP
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
		writeFile << "D=A" << endl << "@R13" << endl << "M=D" << endl;
		popFromStack();
		writeFile << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		asmLine += 6;
	}
}

void codewriter::close(void){
	writeFile << "(END)" << endl << "@END" << endl << "0;JMP" << endl;

	writeFile << "(EQ)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;
	writeFile << "(GT)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;
	writeFile << "(LT)" << endl << "M=0" << endl << "@" << asmLine << endl << "0;JMP" << endl;

	writeFile.close();
}


void codewriter::writerInit(void){
	writeFile << "@256" << endl << "D=A" << endl << "@SP" << endl << "M=D" << endl;
	asmLine += 4;
	writeCall("Sys.init", 0);
}

void codewriter::writeLabel(string label){
	writeFile << "(" << curFuncName << "$" << label << ")" << endl;
	asmLine++;
}

void codewriter::writeGoto(string label){
	writeFile << "@" << curFuncName << "$" << label << endl;
	writeFile << "0;JMP" << endl;
	asmLine += 2;
}

void codewriter::writeIf(string label){
	popFromStack();
	writeFile << "@" << curFuncName << "$" << label << endl;
	writeFile << "D;JNE" << endl;
	asmLine += 2;
}

void codewriter::writeCall(string functionName, int numArgs){
	// push return-address
	writeFile << "@FUNCRETURN" << funcNum << endl;
	writeFile << "D=A" << endl;
	asmLine += 2;
	pushToStack();

	// push LCL, ARG, THIS, THAT
	for (int segName = 1; segName < 5; segName++) {
		writeFile << "@" << segName << endl << "D=M" << endl;
		asmLine += 2;
		pushToStack();
	}

	// ARG = SP-n-5
	writeFile << "@SP" << endl << "D=M" << endl;
	writeFile << "@" << numArgs + 5 << endl << "D=D-A" << endl;
	writeFile << "@ARG" << endl << "M=D" << endl;
	asmLine += 6;

	// LCL = SP
	writeFile << "@SP" << endl << "D=M" << endl;
	writeFile << "@LCL" << endl << "M=D" << endl;
	asmLine += 4;

	// goto f
	writeFile << "@" << functionName << endl;
	writeFile << "0;JMP" << endl;
	asmLine += 2;

	// (return-address)
	writeFile << "(FUNCRETURN" << funcNum << ")" << endl;
	funcNum++; 
	asmLine++;
}

void codewriter::writeReturn(void){
	// FRAME => R13
	// RET => R14

	// FRAME(R13) = LCL
	writeFile << "@LCL" << endl << "D=M" << endl;
	writeFile << "@R13" << endl << "M=D" << endl;
	asmLine += 4;

	// RET(R14) = *(FRAME-5)
	writeFile << "@5" << endl << "A=D-A" << endl << "D=M" << endl;
	writeFile << "@14" << endl << "M=D" << endl;
	asmLine += 5;

	// *ARG = pop()
	popFromStack();
	writeFile << "@ARG" << endl << "A=M" << endl << "M=D" << endl;
	asmLine += 3;

	// SP = ARG+1
	writeFile << "@ARG" << endl << "D=M" << endl << "@SP" << endl << "M=D+1" << endl;
	asmLine += 4;

	// THAT = *(FRAME(R13)-1)
	// THIS = *(FRAME(R13)-2)
	// ARG = *(FRAME(R13)-3)
	// LCL = *(FRAME(R13)-4)
	for (int i = 1, int segName=4; i < 5; i++, segName--) {
		writeFile << "@R13" << endl << "D=M" << endl;
		writeFile << "@" << i << endl << "A=D-A" << endl << "D=M" << endl;
		writeFile << "@" << segName << endl << "M=D" << endl;
		asmLine += 7;
	}

	// goto RET(R14)
	writeFile << "@R14" << endl << "A=M" << endl << "0;JMP" << endl;
	asmLine += 3;
}

void codewriter::writeFunction(string functionName, int numLocals){
	// (f)
	writeFile << "(" << functionName << ")" << endl;
	asmLine++;

	// repeat k times: PUSH 0
	for (int i = 0; i < numLocals; i++) {
		writeFile << "D=0" << endl;
		pushToStack();
	}
}


void codewriter::pushToStack(void){
	writeFile << "@SP" << endl << "A=M" << endl << "M=D" << endl;
	writeFile << "@SP" << endl << "M=M+1" << endl;
	asmLine += 5;
}

void codewriter::popFromStack(void){
	writeFile << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;
	asmLine += 4;
}