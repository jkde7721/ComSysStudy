package comsys.compiler;

import comsys.compiler.enums.*;

import java.io.*;

public class VMWriter {

    BufferedWriter bufferedWriter;

    public VMWriter(String filename) {
        try {
            bufferedWriter = new BufferedWriter(new FileWriter(filename));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void writePush(Segment segment, int index) {
        writeFile("push " + segment.getValue() + " " + index);
    }

    public void writePop(Segment segment, int index) {
        writeFile("pop " + segment.getValue() + " " + index);
    }

    public void writeArithmetic(Command command) {
        writeFile(command.getValue());
    }

    public void writeLabel(String label) {
        writeFile("label " + label);
    }

    public void writeGoto(String label) {
        writeFile("goto " + label);
    }

    public void writeIf(String label) {
        writeFile("if-goto " + label);
    }

    public void writeCall(String name, int nArgs) {
        writeFile("call " + name + " " + nArgs);
    }

    public void writeFunction(String name, int nLocals) {
        writeFile("function " + name + " " + nLocals);
    }

    public void writeReturn() {
        writeFile("return");
    }

    void writeFile(String data) {
        try {
            bufferedWriter.write(data + "\n");
            bufferedWriter.flush();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    public void close() {
        try {
            bufferedWriter.flush();
            bufferedWriter.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
