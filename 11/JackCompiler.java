package comsys.compiler;

import java.io.File;
import java.util.Arrays;

public class JackCompiler {

    public static void main(String[] args) {
        String filePath = "ComplexArrays\\Main.jack";

        JackCompiler analyzer = new JackCompiler();
        analyzer.analyze(filePath);
    }

    JackTokenizer tokenizer;
    CompilationEngine engine;
    VMWriter vmWriter;
    static String prePath = "C:\\Users\\PC\\Desktop\\ComSysStudy\\nand2tetris\\projects\\11\\";

    void analyze(String filePath) {
        final File file = new File(prePath + filePath);

        if(file.isDirectory()) {
            Arrays.stream(file.list()).filter(f -> f.matches(".*\\.jack")).forEach(f -> {
                compile(file.getAbsolutePath() + "\\" + f
                        , file.getAbsolutePath() + "\\" + f.replace(".jack", ".vm"));
            });
        } else {
            compile(file.getAbsolutePath(), file.getAbsolutePath().replace(".jack", ".vm"));
        }
    }

    void compile(String inputFilePath, String outputFilePath) {
        try {
            tokenizer = new JackTokenizer(inputFilePath);
            vmWriter = new VMWriter(outputFilePath);
            engine = new CompilationEngine(tokenizer, vmWriter);

            tokenizer.tokenize();
            engine.startEngine();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
