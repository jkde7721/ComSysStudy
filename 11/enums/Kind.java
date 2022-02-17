package comsys.compiler.enums;

public enum Kind {
    STATIC(0), FIELD(0),
    ARG(0), VAR(0),
    NONE(0);

    private int index;

    Kind(int index) {
        this.index = index;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public void addIndex() {
        index += 1;
    }

    public static void initAllIndex() {
        STATIC.setIndex(0);
        FIELD.setIndex(0);
        ARG.setIndex(0);
        VAR.setIndex(0);
    }
}
