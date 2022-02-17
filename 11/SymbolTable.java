package comsys.compiler;

import comsys.compiler.enums.Range;
import comsys.compiler.identifier.Identifier;
import comsys.compiler.enums.Kind;

import java.util.HashMap;

public class SymbolTable {

    // 키 : 식별자 이름
    public static HashMap<String, Identifier> classRangeTable = new HashMap<>();
    public static HashMap<String, Identifier> subroutineRangeTable = new HashMap<>();

    public SymbolTable() {}

    public static void init() {
        classRangeTable.clear();
        subroutineRangeTable.clear();
        Kind.initAllIndex();
    }

    // 새로운 서브루틴 범위 시작
    public static void startSubroutine() {
        subroutineRangeTable.clear();
        Kind.ARG.setIndex(0);
        Kind.VAR.setIndex(0);
    }

    public static void define(String name, String type, Kind kind) {
        Identifier identifier = new Identifier(name, type, kind, kind.getIndex());
        kind.addIndex(); // 인덱스 +1 증가

        if(kind == Kind.STATIC || kind == Kind.FIELD) {
            classRangeTable.put(name, identifier);
        } else {
            subroutineRangeTable.put(name, identifier);
        }
    }

    public static int varCount(Kind kind) {
        if(kind == Kind.STATIC || kind == Kind.FIELD) {
            return (int) classRangeTable.values().stream()
                    .filter(id -> id.getKind() == kind)
                    .count();
        } else {
            return (int) subroutineRangeTable.values().stream()
                    .filter(id -> id.getKind() == kind)
                    .count();
        }
    }

    public static Kind kindOf(String name) {
        Identifier identifier;

        if(CompilationEngine.curRange == Range.SUBROUTINE) {
            identifier = subroutineRangeTable.get(name);
            if(identifier != null) {
                return identifier.getKind();
            }
        }
        identifier = classRangeTable.get(name);
        return (identifier != null) ? identifier.getKind() : Kind.NONE;
    }

    public static String typeOf(String name) {
        Identifier identifier;

        if(CompilationEngine.curRange == Range.SUBROUTINE) {
            identifier = subroutineRangeTable.get(name);
            if(identifier != null) {
                return identifier.getType();
            }
        }
        identifier = classRangeTable.get(name);
        return (identifier != null) ? identifier.getType() : null;
    }

    public static int indexOf(String name) {
        Identifier identifier;

        if(CompilationEngine.curRange == Range.SUBROUTINE) {
            identifier = subroutineRangeTable.get(name);
            if(identifier != null) {
                return identifier.getIndex();
            }
        }
        identifier = classRangeTable.get(name);
        return (identifier != null) ? identifier.getIndex() : -1;
    }
}
