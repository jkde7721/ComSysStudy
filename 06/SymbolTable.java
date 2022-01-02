package comsys;

import java.util.HashMap;
import java.util.Set;

public class SymbolTable {
    /*
    public static void main(String[] args) {
        Parser parser = new Parser("C:/Users/PC/Desktop/Prog.asm");
        parser.parse();

        SymbolTable symbolTable = new SymbolTable(parser);
        symbolTable.path1();
        symbolTable.path2();
        Set<String> keys = symbolTable.table.keySet();

        for(String key : keys) {
            System.out.println("key : " + key + ", value : " + symbolTable.table.get(key));
        }
    }
    */

    public HashMap<String, Integer> table;
    public Parser parser;

    SymbolTable(Parser parser) {
        table = new HashMap<>();
        // 선언 기호 처리
        table.put("SP", 0);
        table.put("R0", 0);
        table.put("LCL", 1);
        table.put("R1", 1);
        table.put("ARG", 2);
        table.put("R2", 2);
        table.put("THIS", 3);
        table.put("R3", 3);
        table.put("THAT", 4);
        table.put("R4", 4);
        table.put("R5", 5);
        table.put("R6", 6);
        table.put("R7", 7);
        table.put("R8", 8);
        table.put("R9", 9);
        table.put("R10", 10);
        table.put("R11", 11);
        table.put("R12", 12);
        table.put("R13", 13);
        table.put("R14", 14);
        table.put("R15", 15);
        table.put("SCREEN", 16384);
        table.put("KBD", 24576);

        this.parser = parser;
    }

    public void makeTable() {
        path1();
        path2();
    }

    // 레이블 처리
    void path1() {
        int cmdNum = -1; // 현재까지 읽은 명령어 라인의 번호

        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);
            if(parser.commandType(cmd) == Parser.L_COMMAND) {
                String sbl = parser.symbol(cmd);
                if(!contains(sbl)) addEntry(sbl, cmdNum + 1);
            }
            else cmdNum++; // L_COMMAND 가 아닐 때에만 증가
        }
    }

    // 변수 처리
    void path2() {
        int varAddress = 16; // 변수 할당 시작주소

        for(int i = 0; i < parser.commands.size(); i++) {
            String cmd = parser.commands.get(i);
            if(parser.commandType(cmd) == Parser.A_COMMAND) {
                String var = parser.symbol(cmd);
                if(!var.matches("[0-9]+")) {
                    // A 명령어이면서 숫자가 아닌 기호이면서 테이블에 없다면
                    if(!contains(var)) addEntry(var, varAddress++);
                }
            }
        }
    }

    void addEntry(String symbol, int address) {
        table.put(symbol, address);
    }

    boolean contains(String symbol) {
        return table.containsKey(symbol);
    }

    int getAddress(String symbol) {
        if (contains(symbol)) return table.get(symbol);
        return 0;
    }
}
