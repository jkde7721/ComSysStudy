package comsys.vm;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class CodeWriter {
    BufferedWriter bufferedWriter = null;
    String curFilename;
    String curFunctionName;
    int labelNum = 0; // eq, gt, lt
    int returnNum = 0; // return-address

    public CodeWriter(String filename) {
        try {
            File file = new File(filename + ".asm");
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream);
            bufferedWriter = new BufferedWriter(outputStreamWriter);
        } catch(IOException ignored) {}
    }

    // parsing 한 명령어 전달
    public void codeWrite(ArrayList<String> vmCommands, String vmFilename) {
        try {
            setFileName(vmFilename); // 현재 codeWrite 중인 파일 이름 설정
            for (String cmd : vmCommands) {
                int cmdType = Parser.commandType(cmd);
                if (cmdType == Parser.C_ARITHMETIC) writerArithmetic(cmd);
                else if (cmdType == Parser.C_PUSH || cmdType == Parser.C_POP) {
                    writePushPop(cmdType, Parser.arg1(cmd), Parser.arg2(cmd));
                }
                else if(cmdType == Parser.C_LABEL) writeLabel(Parser.arg1(cmd));
                else if(cmdType == Parser.C_GOTO) writeGoto(Parser.arg1(cmd));
                else if(cmdType == Parser.C_IF) writeIf(Parser.arg1(cmd));
                else if(cmdType == Parser.C_CALL) writeCall(Parser.arg1(cmd), Parser.arg2(cmd));
                else if(cmdType == Parser.C_RETURN) writeReturn();
                else if(cmdType == Parser.C_FUNCTION) writeFunction(Parser.arg1(cmd), Parser.arg2(cmd));
                else throw new WrongCmdTypeException();
            }
        } catch(WrongCmdTypeException e) {System.out.println(e.getMessage());}
    }

    // 코드 작성기에게 새로운 VM 파일 번역이 시작되었음을 알림
    void setFileName(String curFilename) {
        String[] names = curFilename.split("/");
        this.curFilename = names[names.length - 1];
    }

    // VM 초기화 (부트스트랩 코드)
    void writerInit() {
        // SP=256
        // call Sys.init
        writeCommand("@256");
        writeCommand("D=A");
        writeCommand("@SP");
        writeCommand("M=D");
        writeCall("Sys.init", 0);
    }

    // 산술 및 논리 명령
    // 단항연산 : neg,not => pop, push
    // 이항연산 : add, sub, eq, gt, lt, and, or => pop, pop, push
    void writerArithmetic(String command) throws WrongCmdTypeException {
        String arithmetic = Parser.arg1(command);

        // 단항연산
        if(arithmetic.equals("neg") || arithmetic.equals("not")) {
            writePushPop(Parser.C_POP, "temp", 7);
            // 연산 수행
            // ex. not
            writeCommand("@R12");
            writeCommand(arithmetic.equals("not") ? "M=!M" : "M=-M");
            writePushPop(Parser.C_PUSH, "temp", 7);
        }
        // 이항연산
        else {
            writePushPop(Parser.C_POP, "temp", 7); // R12
            writePushPop(Parser.C_POP, "temp", 6); // R11
            // 연산 수행
            // ex. add, sub, and, or
            if(arithmetic.equals("add") || arithmetic.equals("sub") || arithmetic.equals("and") || arithmetic.equals("or")) {
                writeCommand("@R11");
                writeCommand("D=M");
                writeCommand("@R12");
                String op = getOperator(arithmetic);
                writeCommand("M=D" + op + "M"); // add : "M=D+M", sub : "M=D-M", and : "M=D&M", or : "M=D|M"
            }
            // ex. eq, gt, lt
            else {
                writeCommand("@R11");
                writeCommand("D=M");
                writeCommand("@R12");
                writeCommand("D=D-M");
                writeCommand("@TRUE" + labelNum);
                String jump = getJump(arithmetic);
                writeCommand("D;" + jump); // eq : JEQ, gt : JGT, lt : JLT
                writeCommand("@R12");
                writeCommand("M=0"); // false
                writeCommand("@PUSH" + labelNum);
                writeCommand("0;JMP");
                writeCommand("(TRUE" + labelNum + ")");
                writeCommand("@R12");
                writeCommand("M=-1"); // true
                writeCommand("(PUSH" + (labelNum++) + ")");
            }
            writePushPop(Parser.C_PUSH, "temp", 7);
        }
    }

    // 메모리 접근 명령
    // command : C_PUSH or C_POP
    // segment : argument, local, static, constant, this, that, pointer, temp
    void writePushPop(int cmdType, String segment, int index) throws WrongCmdTypeException {
        String[] seg1 = {"local", "argument", "this", "that"};
        String[] seg2 = {"pointer", "temp"};
        String seg3 = "constant";
        String seg4 = "static";
        String ram= getRam(segment); // ex. LCL, ARG, R5 ...
        String asmCmd = (segment.equals("pointer") || segment.equals("temp")) ? "D=A" : "D=M";

        if(cmdType == Parser.C_PUSH) {
            if(Arrays.asList(seg1).contains(segment) || Arrays.asList(seg2).contains(segment)) {
                writeCommand("@" + ram);
                writeCommand(asmCmd);
                writeCommand("@" + index);
                writeCommand("A=D+A");
                writeCommand("D=M");
                addStackPushCmd();
            } else if(segment.equals(seg3) || segment.equals(seg4)) {
                if(segment.equals(seg3)) {
                    writeCommand("@" + index);
                    writeCommand("D=A");
                } else {
                    writeCommand("@" + curFilename + "." + index);
                    writeCommand("D=M");
                }
                addStackPushCmd();
            } else {
                throw new WrongCmdTypeException();
            }
        }

        else {
            if(Arrays.asList(seg1).contains(segment) || Arrays.asList(seg2).contains(segment)) {
                writeCommand("@" + ram);
                writeCommand(asmCmd);
                writeCommand("@" + index);
                writeCommand("D=D+A");
                writeCommand("@R13");
                writeCommand("M=D");
                addStackPopCmd();
                writeCommand("@R13");
                writeCommand("A=M");
                writeCommand("M=D");
            } else if(segment.equals(seg4)) {
                addStackPopCmd();
                writeCommand("@" + curFilename + "." + index);
                writeCommand("M=D");
            } else {
                throw new WrongCmdTypeException();
            }
        }

    }

    // functionName$label : 함수 내 라벨만 처리 ex. '(함수이름)'은 처리X
    void writeLabel(String label) {
        // (함수이름$라벨이름)
        writeCommand("(" + curFunctionName + "." + label + ")");
    }

    // 함수 내에서의 goto 만 (함수 간 제어 이동 포함X)
    void writeGoto(String label) {
        writeCommand("@" + curFunctionName + "." + label);
        writeCommand("0;JMP");
    }

    void writeIf(String label) throws WrongCmdTypeException {
        writePushPop(Parser.C_POP, "temp", 7);
        writeCommand("@R12");
        writeCommand("D=M");
        writeCommand("@" + curFunctionName + "." + label); // 스택 최상단 값이 true(0이 아니면)이면 jump
        writeCommand("D;JNE");
    }

    void writeCall(String functionName, int numArgs) {
        // 호출자 프레임 스택에 저장
        String[] callerFrame = {"LCL", "ARG", "THIS", "THAT"};
        writeCommand("@" + curFunctionName + "." + "RETURN" + returnNum);
        writeCommand("D=A");
        addStackPushCmd();
        for(String frame : callerFrame) {
            writeCommand("@" + frame);
            writeCommand("D=M");
            addStackPushCmd();
        }
        // ARG 위치 재설정
        writeCommand("@SP");
        writeCommand("D=M");
        writeCommand("@" + numArgs);
        writeCommand("D=D-A");
        writeCommand("@5");
        writeCommand("D=D-A");
        writeCommand("@ARG");
        writeCommand("M=D");
        // LCL 위치 재설정
        writeCommand("@SP");
        writeCommand("D=M");
        writeCommand("@LCL");
        writeCommand("M=D");
        // 제어 이동
        writeCommand("@" + functionName);
        writeCommand("0;JMP");
        writeLabel("RETURN" + (returnNum++));
    }

    void writeReturn() throws WrongCmdTypeException {
        String[] callerFrame = {"THAT", "THIS", "ARG", "LCL"};

        // frame은 임시 변수
        writeCommand("@LCL");
        writeCommand("D=M");
        writeCommand("@frame");
        writeCommand("M=D");
        // return-address를 임시 변수에 넣음
        writeCommand("@5");
        writeCommand("A=D-A");
        writeCommand("D=M");
        writeCommand("@ret");
        writeCommand("M=D");
        // 호출자를 위해 반환값 위치 재설정
        writePushPop(Parser.C_POP, "temp", 7);
        writeCommand("@R12");
        writeCommand("D=M");
        writeCommand("@ARG");
        writeCommand("A=M");
        writeCommand("M=D");
        // 호출자의 SP 복구
        writeCommand("@ARG");
        writeCommand("D=M+1");
        writeCommand("@SP");
        writeCommand("M=D");
        // 호출자의 THAT, THIS, ARG, LCL 복구
        for(int i = 1; i <= callerFrame.length; i++) {
            writeCommand("@frame");
            writeCommand("D=M");
            writeCommand("@" + i);
            writeCommand("A=D-A");
            writeCommand("D=M");
            writeCommand("@" + callerFrame[i-1]);
            writeCommand("M=D");
        }
        // return-address 로 점프
        writeCommand("@ret");
        writeCommand("A=M");
        writeCommand("0;JMP");
    }

    void writeFunction(String functionName, int numLocals) throws WrongCmdTypeException {
        labelNum = 0;
        returnNum = 0;
        curFunctionName = functionName;

        writeCommand("(" + functionName + ")");
        // 함수 호출 시, LCL, SP는 같기 때문에 그냥 인수 개수만큼 0을 스택에 push 해도 됨
        for(int i = 0; i < numLocals; i++) {
            writePushPop(Parser.C_PUSH, "constant", 0);
        }

        /*
        // 지역변수 개수가 0일 때에는 for 문 앞뒤 라인 의미X
            addCommand("@LCL");
            addCommand("D=M");
            for(int i = 0; i < numLocals; i++) {
                addCommand("@" + i);
                addCommand("A=D+A");
                addCommand("M=0"); // 각 지역변수 모두 0으로 초기화
            }
            addCommand("@" + numLocals);
            addCommand("D=A");
            addCommand("@SP");
            addCommand("M=M+D"); // 스택 포인터 지역변수 개수만큼 증가
        */
    }

    String getOperator(String arithmetic) {
        switch (arithmetic) {
            case "add": return "+";
            case "sub": return "-";
            case "and": return "&";
            case "or": return "|";
            default: return "";
        }
    }

    String getJump(String arithmetic) {
        switch (arithmetic) {
            case "eq": return "JEQ";
            case "gt": return "JGT";
            case "lt": return "JLT";
            default: return "";
        }
    }

    String getRam(String segment) {
        switch (segment) {
            case "local": return "LCL";
            case "argument": return "ARG";
            case "this":
            case "pointer":
                return "THIS";
            case "that": return "THAT";
            case "temp": return "R5";
            default: return "";
        }
    }

    void writeCommand(String cmd) {
        try {
            bufferedWriter.write(cmd + "\n");
        } catch (IOException e) {}
    }

    // D 레지스터에 해당 세그먼트에서 읽어온 값 저장
    void addStackPushCmd() {
        writeCommand("@SP");
        writeCommand("M=M+1");
        writeCommand("A=M-1");
        writeCommand("M=D");
    }

    // D 레지스터에 스택 최상위 값 저장
    void addStackPopCmd() {
        writeCommand("@SP");
        writeCommand("AM=M-1");
        writeCommand("D=M");
    }

    void addEndCmd() {
        writeCommand("(END)");
        writeCommand("@END");
        writeCommand("0;JMP");
    }

    void close() {
        try {
            bufferedWriter.flush();
            bufferedWriter.close();
        } catch(IOException e) {}
    }
}
