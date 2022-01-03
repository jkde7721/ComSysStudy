package comsys;

import java.io.*;
import java.util.ArrayList;
import java.util.Set;

public class Assembler {
    public static void main(String[] args) {
        Assembler assembler = new Assembler();
        assembler.assemble();

        /*
        Set<String> keys = symbolTable.table.keySet();
        for(String key : keys) {
            System.out.println("key : " + key + ", value : " + symbolTable.table.get(key));
        }
        */
    }

    BufferedWriter bufferedWriter = null;
    ArrayList<String> binCommands = new ArrayList<>();
    static Parser parser;
    static Code code;
    static SymbolTable symbolTable;

    public Assembler() {
        try {
            File file = new File("C:/Users/PC/Desktop/Prog.hack");
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream);
            bufferedWriter = new BufferedWriter(outputStreamWriter);

            parser = new Parser("C:/Users/PC/Desktop/Prog.asm");
            code = new Code();
            symbolTable = new SymbolTable();
        } catch(IOException e) {}
    }

    // 어셈블리 코드 -> 이진 코드
    public void assemble() {
        parser.parse(); // 명령어 분석
        pass1();
        pass2();
        writeFile(); // 파일에 저장
    }

    // 레이블 처리
    void pass1() {
        int cmdNum = -1; // 현재까지 읽은 명령어 라인의 번호

        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);
            int cmdType = parser.commandType(cmd);

            if(cmdType == Parser.L_COMMAND) {
                String sbl = parser.symbol(cmd);
                if(!symbolTable.contains(sbl)) symbolTable.addEntry(sbl, cmdNum + 1);
            }
            else cmdNum++; // L_COMMAND 가 아닐 때에만 증가
        }
    }

    // 변수 처리 & 이진 코드로 변환
    void pass2() {
        int varAddress = 16; // 변수 할당 시작주소
        String binCmd = "";

        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);
            int cmdType = parser.commandType(cmd);

            if(cmdType == Parser.A_COMMAND) {
                String var = parser.symbol(cmd);
                if(!var.matches("[0-9]+")) {
                    // A 명령어이면서 숫자가 아닌 기호이면서 테이블에 없다면
                    if(!symbolTable.contains(var)) symbolTable.addEntry(var, varAddress++);
                    binCmd = "0" + getBinNum(symbolTable.getAddress(var)); // 기호
                }
                else binCmd = "0" + getBinNum(Integer.parseInt(var)); // 기호가 아닌 숫자
            }
            else if(cmdType == Parser.C_COMMAND) {
                binCmd = "111" + code.comp(parser.comp(cmd)) + code.dest(parser.dest(cmd)) + code.jump(parser.jump(cmd));
            }
            // L_COMMAND 인 경우는 무시
            else continue;

            binCommands.add(binCmd);
        }
    }

    String getBinNum(int num) {
        StringBuilder result = new StringBuilder();
        while(num != 0) {
            result.append(num % 2);
            num /= 2;
        }
        while(result.length() < 15) {
            result.append(0);
        }
        result = result.reverse();
        return result.toString();
    }

    void writeFile() {
        try {
            for (int i = 0; i < binCommands.size(); i++) {
                bufferedWriter.write(binCommands.get(i) + "\n");
            }
            bufferedWriter.flush();
        } catch(IOException e) {}
    }
}
