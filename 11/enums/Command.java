package comsys.compiler.enums;

public enum Command {
    ADD("add"), SUB("sub"), NEG("neg"),
    EQ("eq"), GT("gt"), LT("lt"),
    AND("and"), OR("or"), NOT("not");

    private String value;

    Command(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }
}
