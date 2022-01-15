package comsys.vm;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class CodeWriter {
    public ArrayList<String> vmCommands = new ArrayList<>();
    public ArrayList<String> asmCommands = new ArrayList<>();
    BufferedWriter bufferedWriter = null;
    String curFilename;
    int labelNum = 0;

    public CodeWriter(String filename, ArrayList<String> commands) {
        try {
            File file = new File("C:/Users/PC/Desktop/ComSysStudy/nand2tetris/projects/07" + filename + ".asm");
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream);
            bufferedWriter = new BufferedWriter(outputStreamWriter);
            this.vmCommands = commands; // parsing 한 명령어 전달
            setFileName(filename);
        } catch(IOException ignored) {}
    }

    public void codeWrite() {
        try {
            for (String cmd : vmCommands) {
                int cmdType = Parser.commandType(cmd);
                if (cmdType == Parser.C_ARITHMETIC) writerArithmetic(cmd);
                else if (cmdType == Parser.C_PUSH || cmdType == Parser.C_POP) {
                    writePushPop(cmdType, Parser.arg1(cmd), Integer.parseInt(Parser.arg2(cmd)));
                }
            }
            addEndCmd();
            writeFile();
            close();
        } catch(WrongCmdTypeException e) {System.out.println(e.getMessage());}
    }

    // 코드 작성기에게 새로운 VM 파일 번역이 시작되었음을 알림?
    void setFileName(String curFilename) {
        String[] names = curFilename.split("/");
        this.curFilename = names[names.length - 1];
    }

    // 산술 및 논리 명령
    // 단항연산 : neg,not => pop, push
    // 이항연산 : add, sub, eq, gt, lt, and, or => pop, pop, push
    void writerArithmetic(String command) throws WrongCmdTypeException {
        String arithmetic = Parser.arg1(command);

        // 단항연산
        if(arithmetic.equals("neg") || arithmetic.equals("not")) {
            writePushPop(Parser.C_POP, "temp", 0);
            // 연산 수행
            // ex. not
            addCommand("@R5");
            addCommand(arithmetic.equals("not") ? "M=!M" : "M=-M");
            writePushPop(Parser.C_PUSH, "temp", 0);
        }
        // 이항연산
        else {
            writePushPop(Parser.C_POP, "temp", 0); // R5
            writePushPop(Parser.C_POP, "temp", 1); // R6
            // 연산 수행
            // ex. add, sub, and, or
            if(arithmetic.equals("add") || arithmetic.equals("sub") || arithmetic.equals("and") || arithmetic.equals("or")) {
                addCommand("@R6");
                addCommand("D=M");
                addCommand("@R5");
                String op = getOperator(arithmetic);
                addCommand("M=D" + op + "M"); // add : "M=D+M", sub : "M=D-M", and : "M=D&M", or : "M=D|M"
            }
            // ex. eq, gt, lt
            else {
                addCommand("@R6");
                addCommand("D=M");
                addCommand("@R5");
                addCommand("D=D-M");
                addCommand("@TRUE" + labelNum);
                String jump = getJump(arithmetic);
                addCommand("D;" + jump); // eq : JEQ, gt : JGT, lt : JLT
                addCommand("@R5");
                addCommand("M=0"); // false
                addCommand("@PUSH" + labelNum);
                addCommand("0;JMP");
                addCommand("(TRUE" + labelNum + ")");
                addCommand("@R5");
                addCommand("M=-1"); // true
                addCommand("(PUSH" + (labelNum++) + ")");
            }
            writePushPop(Parser.C_PUSH, "temp", 0);
        }
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

        // PUSH
        /*
        // local, argument, this, that
            @LCL
            D=M
            @index(인수)
            A=D+A
            D=M
            @SP => 스택 최상단 주소값
            M=M+1
            A=M-1
            M=D

        // pointer, temp
            @THIS // R5
            D=A
            @index
            A=D+A
            D=M
            @SP
            M=M+1
            A=M-1
            M=D

        // constant
            @index
            D=A
            @SP
            M=M+1
            A=M-1
            M=D

        // static
            @curFilename.index
            D=M
            @SP
            M=M+1
            A=M-1
            M=D
        */
        if(cmdType == Parser.C_PUSH) {
            if(Arrays.asList(seg1).contains(segment) || Arrays.asList(seg2).contains(segment)) {
                addCommand("@" + ram);
                addCommand(asmCmd);
                addCommand("@" + index);
                addCommand("A=D+A");
                addCommand("D=M");
                addStackPushCmd();
            } else if(segment.equals(seg3) || segment.equals(seg4)) {
                if(segment.equals(seg3)) {
                    addCommand("@" + index);
                    addCommand("D=A");
                } else {
                    addCommand("@" + curFilename + "." + index);
                    addCommand("D=M");
                }
                addStackPushCmd();
            } else {
                throw new WrongCmdTypeException();
            }
        }

        // POP
        /*
        // local, argument, this, that
            @LCL
            D=M
            @index(인수)
            D=D+A // D는 주소값
            @R13
            M=D
            @SP
            AM=M-1
            D=M // D는 스택 최상단 값
            @R13
            A=M
            M=D

        // pointer, temp
           @THIS
           D=A
           @index
           D=D+A
           @R13
           M=D
           @SP
           AM=M-1
           D=M
           @R13
           A=M
           M=D

        // constant
            불가능

        // static
            @SP
            AM=M-1
            D=M
            @curFilename.index
            M=D
        */
        else {
            if(Arrays.asList(seg1).contains(segment) || Arrays.asList(seg2).contains(segment)) {
                addCommand("@" + ram);
                addCommand(asmCmd);
                addCommand("@" + index);
                addCommand("D=D+A");
                addCommand("@R13");
                addCommand("M=D");
                addStackPopCmd();
                addCommand("@R13");
                addCommand("A=M");
                addCommand("M=D");
            } else if(segment.equals(seg4)) {
                addStackPopCmd();
                addCommand("@" + curFilename + "." + index);
                addCommand("M=D");
            } else {
                throw new WrongCmdTypeException();
            }
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

    // D 레지스터에 해당 세그먼트에서 읽어온 값 저장
    void addStackPushCmd() {
        addCommand("@SP");
        addCommand("M=M+1");
        addCommand("A=M-1");
        addCommand("M=D");
    }

    // D 레지스터에 스택 최상위 값 저장
    void addStackPopCmd() {
        addCommand("@SP");
        addCommand("AM=M-1");
        addCommand("D=M");
    }

    void addCommand(String cmd) {
        asmCommands.add(cmd);
    }

    void addEndCmd() {
        addCommand("(END)");
        addCommand("@END");
        addCommand("0;JMP");
    }

    void writeFile() {
        try {
            for (String cmd : asmCommands) {
                bufferedWriter.write(cmd + "\n");
            }
            bufferedWriter.flush();
        } catch(IOException ignored) {}
    }

    void close() {
        try {
            bufferedWriter.close();
        } catch(IOException ignored) {}
    }
}
