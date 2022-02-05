#include "CodeWriter.h"
#include <iostream>

CodeWriter::CodeWriter(string path) {
	outputStream.open(path + ".asm");
	writerInit();
}

void CodeWriter::setFileName(filesystem::path fpath) {
	fname = fpath.stem().string();
	string temp;

	Parser parser(fpath);
	while (parser.hasMoreCommands()) {
		parser.Parsing();

		switch (parser.getType()) {
		case C_ARITHMETIC:
			writeArithmetic(parser.getArgv1());
			break;
		case C_PUSH:
		case C_POP:
			writePushPop(parser.getType(), parser.getArgv1(), parser.getArgv2());
			break;
		case C_LABEL:
			if (functionNames.back() == "")	temp = parser.getArgv1();
			else temp = functionNames.back() + "$" + parser.getArgv1();
			writeLabel(temp);
			break;
		case C_GOTO:
			writeGoto(parser.getArgv1());
			break;
		case C_IF:
			writeIf(parser.getArgv1());
			break;
		case C_CALL:
			writeCall(parser.getArgv1(), parser.getArgv2());
			break;
		case C_RETURN:
			writeReturn();
			break;
		case C_FUNCTION:
			writeFunction(parser.getArgv1(), parser.getArgv2());
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
			outputStream << "@" << fname << "." << index << "\nD=M\n";
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
		}
		else if (segment.compare("this") == 0) {
			outputStream << "@THIS\nA=M\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
		}
		else if (segment.compare("that") == 0) {
			outputStream << "@THAT\nA=M\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
		}
		else if (segment.compare("pointer") == 0) {
			outputStream << "@3\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
		}
		else if (segment.compare("temp") == 0) {
			outputStream << "@5\n";
			for (int i = 0;i < index;i++)
				outputStream << "A=A+1\n";
			outputStream << "M=D\n";
		}
		else if (segment.compare("static") == 0) {
			outputStream << "@" << fname << "." << index << "\nM=D\n";
		}
		break;
	}
}
void CodeWriter::writerInit() {
	outputStream << "@256\nD=A\n@SP\nM=D\n";//SP=256
	writeCall("Sys.init", 0);
}

void CodeWriter::writeLabel(string label) {
	outputStream << "(" << label << ")\n";
}

void CodeWriter::writeGoto(string label) {
	outputStream << "@" << functionNames.back() << "$" << label << "\n0;JMP\n";
}

void CodeWriter::writeIf(string label) {
	outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
	outputStream << "@" << functionNames.back() << "$" << label << "\nD;JNE\n";
}

void CodeWriter::writeCall(string functionName, int numArgs) {
	reCnt += 1;
	//push return-address
	outputStream << "@return" << reCnt << "\nD=A\n";
	stackPush();
	//push LCL
	outputStream << "@LCL\nD=M\n";
	stackPush();
	//push ARG
	outputStream << "@ARG\nD=M\n";
	stackPush();
	//push THIS
	outputStream << "@THIS\nD=M\n";
	stackPush();
	//push THAT
	outputStream << "@THAT\nD=M\n";
	stackPush();
	//ARG=SP-n-5
	outputStream << "@SP\nD=M\n@" << numArgs << "\nD=D-A\n@5\nD=D-A\n";
	outputStream << "@ARG\nM=D\n";
	//LCL=SP
	outputStream << "@SP\nD=M\n@LCL\nM=D\n";
	//goto f
	outputStream << "@" << functionName << "\n0;JMP\n";
	//(return-address)
	outputStream << "(return" << reCnt << ")\n";
}

void CodeWriter::writeReturn() {
	//FRAME=LCL
	outputStream << "@LCL\nD=M\n@R13\nM=D\n";
	//RET=*(FRAME-5)
	outputStream << "@5\nA=D-A\nD=M\n@R14\nM=D\n";
	//*ARG=pop() : 현재 ARG가 가리키는 위치에 리턴값 저장
	outputStream << "@SP\nM=M-1\nA=M\nD=M\n";
	outputStream << "@ARG\nA=M\nM=D\n";
	//SP=ARG+1
	outputStream << "@ARG\nD=M+1\n";
	outputStream << "@SP\nM=D\n";
	//THAT=*(FRAME-1)
	outputStream << "@R13\nM=M-1\nA=M\nD=M\n";
	outputStream << "@THAT\nM=D\n";
	//THIS=*(FRAME-2)
	outputStream << "@R13\nM=M-1\nA=M\nD=M\n";
	outputStream << "@THIS\nM=D\n";
	//ARG=*(FRAME-3)
	outputStream << "@R13\nM=M-1\nA=M\nD=M\n";
	outputStream << "@ARG\nM=D\n";
	//LCL=*(FRAME-4)
	outputStream << "@R13\nM=M-1\nA=M\nD=M\n";
	outputStream << "@LCL\nM=D\n";
	//goto RET
	outputStream << "@R14\nA=M\n0;JMP\n";
}

void CodeWriter::writeFunction(string functionName, int numLocals) {
	//(f)
	functionNames.push_back(functionName);
	writeLabel(functionName);
	//repeat k times:
	//PUSH 0
	if (numLocals > 0)outputStream << "@0\nD=A\n";
	for (int i = 0;i < numLocals;i++) stackPush();
}

void CodeWriter::Close() {
	outputStream.close();
}

void CodeWriter::stackPush() {
	outputStream << "@SP\nA=M\nM=D\n";
	outputStream << "@SP\nM=M+1\n";
}