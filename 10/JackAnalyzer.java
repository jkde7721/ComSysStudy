package comsys.compiler;

import java.io.File;
import java.util.Arrays;

public class JackAnalyzer {

    public static void main(String[] args) {
        String filePath = "Square\\";

        JackAnalyzer analyzer = new JackAnalyzer();
        analyzer.analyze(filePath);
    }

    JackTokenizer tokenizer;
    CompilationEngine engine;
    static String prePath = "C:\\Users\\PC\\Desktop\\ComSysStudy\\nand2tetris\\projects\\10\\";

    void analyze(String filePath) {
        final File file = new File(prePath + filePath);

        if(file.isDirectory()) {
            Arrays.stream(file.list()).filter(f -> f.matches(".*\\.jack")).forEach(f -> {
                codeGenerate(file.getAbsolutePath() + "\\" + f
                        , file.getAbsolutePath() + "\\out\\" + f.replace(".jack", ".xml"));
            });
        } else {
            codeGenerate(file.getAbsolutePath(), file.getParent() + "\\out\\" + file.getName().replace(".jack", ".xml"));
        }
    }

    void codeGenerate(String inputFilePath, String outputFilePath) {
        try {
            tokenizer = new JackTokenizer(inputFilePath);
            engine = new CompilationEngine(tokenizer, outputFilePath);

            tokenizer.tokenize();
            engine.compile();
        } catch (Exception e) {}
    }
}
