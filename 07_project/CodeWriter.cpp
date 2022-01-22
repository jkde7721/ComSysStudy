#include "CodeWriter.h"

using namespace std;

CodeWriter::CodeWriter(string f_name) {
	filename = f_name;
}
void CodeWriter::setFileName() {
	string out_file_name = filename.substr(0, filename.length() - 3) + ".asm";
	fout.open(out_file_name);
}
void CodeWriter::writerArithmetic(string command) {
	if (command == "add" || command == "sub" || command == "and" || command == "or"){
		stk_pop();
		decSP();
		op(command);
		incSP();
	}
	else if (command == "neg") {
		stk_pop();
		fout << "D=-D" << endl << "M=D" << endl;
		incSP();
	}
	else if (command == "not") {
		stk_pop();
		fout << "D=!D" << endl << "M=D" << endl;
		incSP();
	}
	else if (command == "eq") {
		/*(eq/lt/gt)Num을 만들어 구분*/
		eqNum++;
		stk_pop();
		decSP();
		fout << "D=M-D" << endl;
		fout << "@EQ" << eqNum << endl;
		fout << "D;JEQ" << endl;

		/*조건을 만족하지 않는 경우 - 스택에 0 넣기(참인 경우는 건너뛰는 명령)*/
		stk_push(0);
		fout << "@ENDEQ" << eqNum << endl;
		fout << "0;JMP" << endl;

		/*조건 만족하면 스택에 -1 넣기*/
		fout << "(EQ" << eqNum << ")" << endl;
		stk_push(-1);

		/*조건 만족하든 안 하든 지남, 그냥 표시만(별다른 코드 필요치 X)*/
		fout << "(ENDEQ" << eqNum << ")" << endl;
	}
	else if (command == "gt") {
		gtNum++;
		stk_pop();
		decSP();
		fout << "D=M-D" << endl;
		fout << "@GT" << gtNum << endl;
		fout << "D;JGT" << endl;
		stk_push(0);
		fout << "@ENDGT" << gtNum << endl;
		fout << "0;JMP" << endl;
		fout << "(GT" << gtNum << ")" << endl;
		stk_push(-1);
		fout << "(ENDGT" << gtNum << ")" << endl;
	}
	else if (command == "lt") {
		ltNum++;
		stk_pop();
		decSP();
		fout << "D=M-D" << endl;
		fout << "@LT" << ltNum << endl;
		fout << "D;JLT" << endl;
		stk_push(0);
		fout << "@ENDLT" << ltNum << endl;
		fout << "0;JMP" << endl;
		fout << "(LT" << ltNum <<")"<< endl;
		stk_push(-1);
		fout << "(ENDLT" << ltNum << ")" << endl;
	}
}
void CodeWriter::stk_push(int x) {
	fout << "@SP" << endl << "A=M" << endl;
	fout << "M=" << x << endl;
	incSP();
}
void CodeWriter::stk_pop() {
	decSP();
	fout << "D=M" << endl;
}
void CodeWriter::decSP() {
	fout << "@SP" << endl << "AM=M-1" << endl;
}
void CodeWriter::incSP() {
	fout << "@SP" << endl << "AM=M+1" << endl;
}
void CodeWriter::op(string command) {
	/*sub의 경우 M-D임에 주의!*/
	if (command == "add") {
		fout<<"D=D"<<"+"<<"M"<<endl<<"M=D"<<endl;
	}
	else if (command == "sub") {
		fout << "D=M" << "-" << "D" << endl << "M=D" << endl;
	}
	else if (command == "and") {
		fout << "D=D" << "&" << "M" << endl << "M=D" << endl;
	}
	else if (command == "or") {
		fout << "D=D" << "|" << "M" << endl << "M=D" << endl;
	}
}
void CodeWriter::WritePushPop(string command, string segment, int index) {
	if (command == "C_PUSH") {
		if(segment == "argument"){
			fout << "@ARG"<< endl << "D = M" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음. 즉, (ARG+index)에 저장된 값
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl; //스택에 D를 push
			incSP();
		}
		else if (segment == "local") {
			fout << "@LCL" << endl << "D = M" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
		else if (segment == "static") {
			fout << "@" << filename << "." << index << endl << "D=M" << endl;
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
		else if (segment == "constant") {
			fout << "@"<<index << endl << "D=A" << endl; //역시 D에 저장할 값이 있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl; //stk_push(D)
			incSP();
		}
		else if (segment == "this") {
			fout << "@THIS" << endl << "D=M" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
		else if (segment == "that") {
			fout << "@THAT" << endl << "D=M" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
		else if (segment == "pointer") {
			fout << "@3" << endl << "D=A" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
		else if (segment == "temp") {
			fout << "@5" << endl << "D=A" << endl;
			fout << "@" << index << endl << "D=D+A" << endl << "A=D" << endl << "D=M" << endl;// -> D에 push해야될 값이 들어있음
			fout << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incSP();
		}
	}
	else if (command == "C_POP") {
		if (segment == "argument") {
			fout << "@ARG" << endl << "D = M" << endl << "@" << index << endl << "D = D + A" << endl; //D는 저장할 주소
			fout << "@R13" << endl << "M=D" << endl; //임시저장
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl; //D는 꺼낸 데이터
		}
		else if (segment == "local") {
			fout << "@LCL" << endl << "D = M" << endl << "@" << index << endl << "D = D + A" << endl;
			fout << "@R13" << endl << "M=D" << endl;
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
		else if (segment == "static") {
			fout << "@" << filename << "." << index << endl;
			fout << "D=A" << endl << "@R13" << endl << "M=D" << endl; //R[13]에 D 임시저장
			stk_pop(); //스택 팝함
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl; //그것을 다시 R13에 저장
		}
		else if (segment == "this") {
			fout << "@THIS" << endl << "D = M" << endl << "@" << index << endl << "D = D + A" << endl;
			fout << "@R13" << endl << "M=D" << endl;
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
		else if (segment == "that") {
			fout << "@THAT" << endl << "D = M" << endl << "@" << index << endl << "D = D + A" << endl;
			fout << "@R13" << endl << "M=D" << endl;
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
		else if (segment == "pointer") {
			fout << "@3" << endl << "D = A" << endl << "@" << index << endl << "D = D + A" << endl;
			fout << "@R13" << endl << "M=D" << endl;
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
		else if (segment == "temp") {
			fout << "@5" << endl << "D = A" << endl << "@" << index << endl << "D = D + A" << endl;
			fout << "@R13" << endl << "M=D" << endl;
			stk_pop();
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
	}
}
void CodeWriter::Close() {
	fout << "(END)" << endl << "@END" << endl << "0;JMP" << endl;
	fout.close();
}