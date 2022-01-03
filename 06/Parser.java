package comsys;

import java.io.*;
import java.util.ArrayList;

public class Parser {
    /*
    public static void main(String[] args) {
        Parser parser = new Parser("C:/Users/PC/Desktop/Prog.asm");
        parser.parse();

        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);;
            System.out.println(cmd);
            // 명령어 타입에 따라 주소 추출 혹은 dest, comp, jump 명령어 추출
            if(parser.commandType(cmd) == A_COMMAND || parser.commandType(cmd) == L_COMMAND) {
                System.out.println(parser.symbol(cmd));
            }
            else {
                System.out.println("dest : " + parser.dest(cmd) + ", comp : " + parser.comp(cmd) + ", jump : " + parser.jump(cmd));
            }
            System.out.println();
        }
    }
    */

    public static final int A_COMMAND = 0;
    public static final int C_COMMAND = 1;
    public static final int L_COMMAND = 2;
    public ArrayList<String> commands = new ArrayList<>();
    BufferedReader bufferedReader = null;
    String curCmd = ""; // 현재 명령어
    boolean comment = false; // 주석 여부

    public Parser(String filename) {
        try {
            File file = new File(filename);
            FileInputStream fileInputStream = new FileInputStream(file);
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            bufferedReader = new BufferedReader(inputStreamReader);
        } catch(IOException e) {}
    }

    public void parse() {
        try {
            // 명령어 한 줄씩 read
            while (hasMoreCommands()) {
                advance();
            }
        } catch(IOException e) {}
    }

    boolean hasMoreCommands() throws IOException {
        bufferedReader.mark(100);
        return bufferedReader.readLine() != null; // 읽을 수 있는지만 확인 (실제로 읽으면 안됨)
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

    public int commandType(String cmd) {
        if(cmd.contains("@")) return A_COMMAND;
        else if(cmd.contains("(")) return L_COMMAND;
        else return C_COMMAND;
    }

    public String symbol(String cmd) {
        String result = "";
        if(commandType(cmd) == A_COMMAND) result = cmd.substring(1);
        else if(commandType(cmd) == L_COMMAND) result = cmd.substring(1, cmd.length() - 1);
        return result;
    }

    public String dest(String cmd) {
        String result = "";
        int idx = cmd.indexOf('=');
        if(idx != -1) result = cmd.substring(0, idx);
        return result;
    }

    public String comp(String cmd) {
        String result = "";
        int idx = cmd.indexOf('=');
        int idx2 = cmd.indexOf(';');
        if(idx != -1) {
            if(idx2 != -1) result = cmd.substring(idx+1, idx2);
            else result = cmd.substring(idx+1);
        } else {
            if(idx2 != -1) result = cmd.substring(0, idx2);
            else result = cmd;
        }
        return result;
    }

    public String jump(String cmd) {
        String result = "";
        int idx = cmd.indexOf(';');
        if(idx != -1) result = cmd.substring(idx + 1);
        return result;
    }
}
