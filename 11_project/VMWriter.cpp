#include "VMWriter.h"

VMWriter::VMWriter(string vmFile) {
	writeFile.open(vmFile);
	tab = "";
}

void VMWriter::writePush(Segment segment, int index) {
	writeFile << tab << "push " << Segment_ele[segment] << " " << index << endl;
}

void VMWriter::writePop(Segment segment, int index) {
	writeFile << tab << "pop " << Segment_ele[segment] << " " << index << endl;
}

void VMWriter::writeArithmetic(Command command) {
	writeFile << tab << Command_ele[command] << endl;
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
