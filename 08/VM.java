package comsys.vm;

import java.io.File;
import java.util.ArrayList;

public class VM {
    ArrayList<Parser> parsers;
    CodeWriter codeWriter;
    static final String prePathName = "C:/Users/PC/Desktop/ComSysStudy/nand2tetris/projects/08";
    static final String[] paths = {"/FunctionCalls/NestedCall/NestedCall", "/FunctionCalls/FibonacciElement", "/FunctionCalls/StaticsTest"};
    //String[] paths = {"/ProgramFlow/BasicLoop/BasicLoop", "/ProgramFlow/FibonacciSeries/FibonacciSeries", "/FunctionCalls/SimpleFunction/SimpleFunction"};

    public static void main(String[] args) {
        /*
        String[] paths = {"/StackArithmetic/SimpleAdd/SimpleAdd", "/StackArithmetic/StackTest/StackTest",
                "/MemoryAccess/BasicTest/BasicTest", "/MemoryAccess/PointerTest/PointerTest",
                "/MemoryAccess/StaticTest/StaticTest"};
        */

        VM vm = new VM();
        vm.vmTranslate();
    }

    public VM() {
        parsers = new ArrayList<>();
    }

    public void vmTranslate() {
        String codeWriterFileName;

        for(String path : paths) {
            File allPath = new File(prePathName + path);
            if(allPath.isDirectory()) {
                String[] fileList = allPath.list(); // 디렉토리의 하위 리스트를 배열 형태로 반환
                assert fileList != null;

                for(String file : fileList) {
                    if(file.matches("^[a-zA-Z0-9]*\\.vm$")) // .vm 파일이면
                        parsers.add(new Parser(prePathName + path + "/" + file.substring(0, file.indexOf(".")))); // Parser 객체 여러개
                }
                codeWriterFileName = prePathName + path + "/" + path.split("/")[2];
            } else {
                parsers.add(new Parser(prePathName + path));
                codeWriterFileName = prePathName + path;
            }

            codeWriter = new CodeWriter(codeWriterFileName); // CodeWriter 객체 1개
            codeWriter.writerInit(); // 부트스트랩 코드
            for(Parser parser : parsers) {
                parser.parse();
                codeWriter.codeWrite(parser.commands, parser.filename);
            }
            parsers.clear();
            codeWriter.close();
        }
    }
}
