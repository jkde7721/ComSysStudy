package comsys.compiler.enums;

public enum Segment {
    CONST("constant"), ARG("argument"), LOCAL("local"), STATIC("static"),
    THIS("this"), THAT("that"), POINTER("pointer"), TEMP("temp");

    private String value;

    Segment(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }
}
