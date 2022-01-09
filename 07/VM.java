package comsys.vm;

public class VM {
    Parser parser;
    CodeWriter codeWriter;

    public static void main(String[] args) {
        String[] filenames = {"/StackArithmetic/SimpleAdd/SimpleAdd", "/StackArithmetic/StackTest/StackTest",
                "/MemoryAccess/BasicTest/BasicTest", "/MemoryAccess/PointerTest/PointerTest",
                "/MemoryAccess/StaticTest/StaticTest"};
        VM vm = new VM();

        for(String filename : filenames) {
            vm.vmTranslate(filename);
        }
    }

    public VM() {}

    public void vmTranslate(String filename) {
        parser = new Parser(filename);
        codeWriter = new CodeWriter(filename, parser.commands);
        parser.parse();
        codeWriter.codeWrite();
    }
}
