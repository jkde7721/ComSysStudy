package comsys.compiler.identifier;

import comsys.compiler.enums.Kind;

public class Identifier {
    private String name;
    private String type;
    private Kind kind;
    private int index;

    public Identifier(String name, String type, Kind kind, int index) {
        this.name = name;
        this.type = type;
        this.kind = kind;
        this.index = index;
    }

    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }

    public Kind getKind() {
        return kind;
    }

    public int getIndex() {
        return index;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setKind(Kind kind) {
        this.kind = kind;
    }

    public void setIndex(int index) {
        this.index = index;
    }
}
