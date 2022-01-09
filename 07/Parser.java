package comsys.vm;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class Parser {
    public static final int C_ARITHMETIC = 0;
    public static final int C_PUSH = 1;
    public static final int C_POP = 2;
    public static final int C_LABEL = 3;
    public static final int C_GOTO = 4;
    public static final int C_IF = 5;
    public static final int C_FUNCTION = 6;
    public static final int C_RETURN = 7;
    public static final int C_CALL = 8;

    ArrayList<String> commands = new ArrayList<>();
    BufferedReader bufferedReader = null;
    String curCmd = "";
    boolean comment = false;

    public Parser(String filename) {
        try {
            File file = new File("C:/Users/PC/Desktop/ComSysStudy/nand2tetris/projects/07" + filename + ".vm");
            FileInputStream fileInputStream = new FileInputStream(file);
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            bufferedReader = new BufferedReader(inputStreamReader);
        } catch(IOException ignored) {}
    }

    public void parse() {
        try {
            while (hasMoreCommands()) {
                advance();
            }
        } catch(IOException ignored) {}
    }

    boolean hasMoreCommands() throws IOException {
        bufferedReader.mark(100);
        return bufferedReader.readLine() != null;
    }

    void advance() throws IOException {
        bufferedReader.reset();
        curCmd = bufferedReader.readLine();
        removeComment();
    }

    // 주석 제거
    void removeComment() {
        int idx;
        if(!comment) {
            if(curCmd.contains("/*")) {
                comment = true; // 여러 줄 주석 시작
                curCmd = "";
            }
            else {
                if((idx = curCmd.indexOf('/')) != -1) { // 주석 위치 인덱스
                    curCmd = curCmd.substring(0, idx);
                }
                curCmd = curCmd.trim();
                if(!curCmd.equals("")) addCommand(curCmd); // 명령어 리스트에 추가
            }
        } else { // 여러 줄 주석의 마지막인지만 확인
            if(curCmd.contains("*/")) comment = false;
            curCmd = "";
        }
    }

    void addCommand(String cmd) {
        commands.add(cmd);
    }

    // C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL
    public static int commandType(String cmd) {
        if(cmd.contains("push")) return C_PUSH;
        else if(cmd.contains("pop")) return C_POP;
        else if(cmd.contains("label")) return C_LABEL;
        else if(cmd.contains("goto")) return C_GOTO;
        else if(cmd.contains("if")) return C_IF;
        else if(cmd.contains("function")) return C_FUNCTION;
        else if(cmd.contains("return")) return C_RETURN;
        else if(cmd.contains("call")) return C_CALL;
        return C_ARITHMETIC;
    }

    // 명령어 타입이 C_RETURN 이면 호출 X, C_ARITHMETIC 이면 명령어 자체 반환
    public static String arg1(String cmd) throws WrongCmdTypeException {
        int cmdType = commandType(cmd);
        if(cmdType == C_RETURN) throw new WrongCmdTypeException();

        String[] args = cmd.split(" ");
        if(cmdType == C_ARITHMETIC) return args[0];
        return args[1]; // 명령어 인수1 인수2 중 인수1 반환
    }

    public static String arg2(String cmd) throws WrongCmdTypeException {
        int[] posCmdTypes = {C_PUSH, C_POP, C_FUNCTION, C_CALL};
        int cmdType = commandType(cmd);
        if(Arrays.stream(posCmdTypes).noneMatch(x -> x == cmdType)) throw new WrongCmdTypeException();

        String[] args = cmd.split(" ");
        return args[2]; // 명령어 인수1 인수2 중 인수2 반환
    }
}
