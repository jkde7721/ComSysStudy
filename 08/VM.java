package comsys.vm;

import java.io.File;
import java.util.ArrayList;

public class VM {
    ArrayList<Parser> parsers;
    CodeWriter codeWriter;
    static final String prePathName = "C:/Users/PC/Desktop/ComSysStudy/nand2tetris/projects/08";
    // 7장
    //String[] paths = {"/StackArithmetic/SimpleAdd/SimpleAdd.vm", "/StackArithmetic/StackTest/StackTest.vm", "/MemoryAccess/BasicTest/BasicTest.vm", "/MemoryAccess/PointerTest/PointerTest.vm", "/MemoryAccess/StaticTest/StaticTest.vm"};
    // 8장
    //String[] paths = {"/ProgramFlow/BasicLoop/BasicLoop.vm", "/ProgramFlow/FibonacciSeries/FibonacciSeries.vm", "/FunctionCalls/SimpleFunction/SimpleFunction.vm"};
    String[] paths = {"/FunctionCalls/NestedCall/NestedCall.vm", "/FunctionCalls/FibonacciElement", "/FunctionCalls/StaticsTest"};

    public static void main(String[] args) {
        VM vm = new VM();
        vm.vmTranslate();
    }

    public VM() {
        parsers = new ArrayList<>();
    }

    public void vmTranslate() {
        String codeWriterFilename;

        for(String path : paths) {
            File allPath = new File(prePathName + path);
            if(allPath.isDirectory()) {
                String[] fileList = allPath.list(); // 디렉토리의 하위 리스트를 배열 형태로 반환
                assert fileList != null;

                for(String file : fileList) {
                    if(file.matches("^[a-zA-Z0-9]*\\.vm$")) // .vm 파일이면
                        parsers.add(new Parser(prePathName + path + "/" + file)); // Parser 객체 여러개
                }
                codeWriterFilename = prePathName + path + "/" + path.split("/")[2] + ".asm";
            } else {
                parsers.add(new Parser(prePathName + path));
                codeWriterFilename = prePathName + path.replace(".vm", ".asm");
            }

            codeWriter = new CodeWriter(codeWriterFilename); // CodeWriter 객체 1개
            codeWriter.writerInit(); // 부트스트랩 코드
            for(Parser parser : parsers) {
                parser.parse();
                codeWriter.codeWrite(parser);
            }
            parsers.clear();
            codeWriter.close();
        }
    }
}
