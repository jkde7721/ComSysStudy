package comsys.compiler.enums;

public enum Keyword {
    CLASS("class"), METHOD("method"), FUNCTION("function"),
    CONSTRUCTOR("constructor"), INT("int"), BOOLEAN("boolean"),
    CHAR("char"), VOID("void"), VAR("var"),
    STATIC("static"), FIELD("field"), LET("let"),
    DO("do"), IF("if"), ELSE("else"),
    WHILE("while"), RETURN("return"),
    TRUE("true"), FALSE("false"),
    NULL("null"), THIS("this"),
    TRASH("");

    private String value;

    private Keyword(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }
}
