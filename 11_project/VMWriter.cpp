#include "VMWriter.h"

VMWriter::VMWriter(string vmFile) {
	writeFile.open(vmFile);
	tab = "";
}

void VMWriter::writePush(Segment segment, int index) {
	writeFile << tab << "push " << segment << " " << index << endl;
}

void VMWriter::writePop(Segment segment, int index) {
	writeFile << tab << "pop " << segment << " " << index << endl;
}

void VMWriter::writeArithmetic(Command command) {
	switch (command) {
	case ADD:
		writeFile << tab << "add" << endl;
		break;
	case SUB:
		writeFile << tab << "sub" << endl;
		break;
	case NEG:
		writeFile << tab << "neg" << endl;
		break;
	case EQ:
		writeFile << tab << "eq" << endl;
		break;
	case GT:
		writeFile << tab << "gt" << endl;
		break;
	case LT:
		writeFile << tab << "lt" << endl;
		break;
	case AND:
		writeFile << tab << "and" << endl;
		break;
	case OR:
		writeFile << tab << "or" << endl;
		break;
	case NOT:
		writeFile << tab << "not" << endl;
	}
}

void VMWriter::writeLabel(string label) {
	writeFile << tab << "label " << label << endl;
	tab += "  ";
}

void VMWriter::writeGoto(string label) {
	writeFile << tab << "goto " << label << endl;
}

void VMWriter::writeIf(string label) {
	writeFile << tab << "if-goto " << label << endl;
}

void VMWriter::writeCall(string name, int nArgs) {
	writeFile << tab << "call " << name << " " << nArgs << endl;
}

void VMWriter::writeFunction(string name, int nLocals) {
	writeFile << tab << "function " << name << " " << nLocals << endl;
	tab += "  ";
}

void VMWriter::writeReturn(void) {
	writeFile << tab << "return" << endl;
	tab.substr(0, tab.length() - 2);
}

void VMWriter::close(void) {
	writeFile.close();
}