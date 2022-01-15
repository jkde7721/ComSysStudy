#include "CodeWriter.h"
#include <iostream>

CodeWriter::CodeWriter() {
	outputStream.open("C:\\Users\\User\\Desktop\\yibin\\nand2tetris\\projects\\07\\MemoryAccess\\StaticTest\\StaticTest.asm");
}

void CodeWriter::setFileName(string fname) {
	Parser parser(fname);
	while (parser.hasMoreCommands()) {
		parser.Parsing();
		/*test code
		string s[] = {
		"C_ARITHMETIC", "C_PUSH", "C_POP", "C_LABEL", "C_GOTO", "C_IF", "C_FUNCTION", "C_RETURN", "C_CALL", "SKIP"
		};
		cout << s[parser.getType()] << " " << parser.getArgv1() << " " << parser.getArgv2() << '\n';
		*/
		switch (parser.getType()) {
		case C_ARITHMETIC:
			writeArithmetic(parser.getArgv1());
			break;
		case C_PUSH:
		case C_POP:
			writePushPop(parser.getType(), parser.getArgv1(), parser.getArgv2());
			break;
		}
	}

	parser.Close();
}

void CodeWriter::writeArithmetic(string command) {
	if (command.compare("add") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=D+M\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("sub") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=M-D\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("neg") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "D=-D\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("eq") == 0) {
		eqCnt += 1;
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=M-D\n@EQ" << eqCnt << "\nD;JEQ\n";
		outputStream << "@SP\nA=M\nM=0\n@ENDEQ" << eqCnt << "\n0;JMP\n";//not equal(false 0)
		outputStream << "(EQ" << eqCnt << ")\n@SP\nA=M\nM=-1\n(ENDEQ" << eqCnt << ")\n";//equal(true -1)
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("gt") == 0) {
		gtCnt += 1;
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=M-D\n@GT" << gtCnt << "\nD;JGT\n";
		outputStream << "@SP\nA=M\nM=0\n@ENDGT" << gtCnt << "\n0;JMP\n";//not greater(false 0)
		outputStream << "(GT" << gtCnt << ")\n@SP\nA=M\nM=-1\n(ENDGT" << gtCnt << ")\n";//greater(true -1)
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("lt") == 0) {
		ltCnt += 1;
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=M-D\n@LT" << ltCnt << "\nD;JLT\n";
		outputStream << "@SP\nA=M\nM=0\n@ENDLT" << ltCnt << "\n0;JMP\n";//not less(false 0)
		outputStream << "(LT" << ltCnt << ")\n@SP\nA=M\nM=-1\n(ENDLT" << ltCnt << ")\n";//less(true -1)
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("and") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=D&M\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("or") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "@SP\nM=M-1\nA=M\nD=D|M\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else if (command.compare("not") == 0) {
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		outputStream << "D=!D\nM=D\n";
		outputStream << "@SP\nM=M+1\n";
	}
	else cout << "Error\n";
}

void CodeWriter::writePushPop(type command, string segment, int index) {
	switch (command) {
	case C_PUSH:
		if (segment.compare("constant") == 0) {
			outputStream << "@" << index << "\nD=A\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("local") == 0) {
			outputStream << "@LCL\nD=M\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("argument") == 0) {
			outputStream << "@ARG\nD=M\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("this") == 0) {
			outputStream << "@THIS\nD=M\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("that") == 0) {
			outputStream << "@THAT\nD=M\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("pointer") == 0) {
			outputStream << "@3\nD=A\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("temp") == 0) {
			outputStream << "@5\nD=A\n@" << index << "\nD=D+A\nA=D\nD=M\n";
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		else if (segment.compare("static") == 0) {
			outputStream << "@Xxx." << index << "\nD=M\n";//파일 이름 처리해야 함
			outputStream << "@SP\nA=M\nM=D\n";
			outputStream << "@SP\nM=M+1\n";
		}
		break;
	case C_POP:
		outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
		if (segment.compare("local") == 0) {
			outputStream << "@LCL\nA=M\n";
			for(int i=0;i<index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
		}
		else if (segment.compare("argument") == 0) {
			outputStream << "@ARG\nA=M\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
			//outputStream << "@ARG\nD=A\n@" << index << "\nD=D+A\nM=D\n";
		}
		else if (segment.compare("this") == 0) {
			outputStream << "@THIS\nA=M\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
			//outputStream << "@THIS\nD=A\n@" << index << "\nD=D+A\nM=D\n";
		}
		else if (segment.compare("that") == 0) {
			outputStream << "@THAT\nA=M\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
			//outputStream << "@THAT\nD=A\n@" << index << "\nD=D+A\nM=D\n";
		}
		else if (segment.compare("pointer") == 0) {
			outputStream << "@3\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
			//outputStream << "@3\nD=A\n@" << index << "\nD=D+A\nM=D\n";
		}
		else if (segment.compare("temp") == 0) {
			outputStream << "@5\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
			//outputStream << "@5\nD=A\n@" << index << "\nD=D+A\nM=D\n";
		}
		else if (segment.compare("static") == 0) {
			outputStream << "@Xxx." << index << "\nM=D\n";
		}
		break;
	}
}

void CodeWriter::Close() {
	outputStream.close();
}