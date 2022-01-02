package comsys;

import java.io.*;
import java.util.ArrayList;

public class Assembler {
    public static void main(String[] args) {
        Assembler assembler = new Assembler();
        assembler.assemble();
    }

    BufferedWriter bufferedWriter = null;
    ArrayList<String> binCommands = new ArrayList<>();
    Parser parser;
    Code code;
    SymbolTable symbolTable;

    Assembler() {
        try {
            File file = new File("C:/Users/PC/Desktop/Prog.hack");
            FileOutputStream fileOutputStream = new FileOutputStream(file);
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(fileOutputStream);
            bufferedWriter = new BufferedWriter(outputStreamWriter);

            parser = new Parser("C:/Users/PC/Desktop/Prog.asm");
            code = new Code();
            symbolTable = new SymbolTable(parser);
        } catch(IOException e) {}
    }

    // 어셈블리 코드 -> 이진 코드
    void assemble() {
        parser.parse(); // 명령어 분석
        symbolTable.makeTable(); // 기호 테이블 생성

        String binCmd = "";
        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);
            int cmdType = parser.commandType(cmd);

            if(cmdType == Parser.A_COMMAND) {
                String var = parser.symbol(cmd);
                if(var.matches("[0-9]+")) binCmd = "0" + getBinNum(Integer.parseInt(var)); // 기호가 아닌 숫자
                else binCmd = "0" + getBinNum(symbolTable.getAddress(var)); // 기호
            }
            else if(cmdType == Parser.C_COMMAND) {
                binCmd = "111" + code.comp(parser.comp(cmd)) + code.dest(parser.dest(cmd)) + code.jump(parser.jump(cmd));
            }
            // L_COMMAND 인 경우는 무시
            else continue;

            binCommands.add(binCmd);
        }

        writeFile(); // 파일에 저장
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
