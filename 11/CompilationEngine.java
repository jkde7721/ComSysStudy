package comsys.compiler;

import comsys.compiler.enums.*;

public class CompilationEngine {
    JackTokenizer tokenizer;
    VMWriter writer;

    static String className;
    static String subroutineName;
    static int labelIdx = 0;
    static Range curRange;

    public CompilationEngine(JackTokenizer tokenizer, VMWriter writer) {
            this.tokenizer = tokenizer;
            this.writer = writer;
    }

    public void startEngine() {
        compileClass();
    }

    void compileClass() {
        curRange = Range.CLASS;
        tokenizer.skipNext(); // 'class'
        className = tokenizer.returnNext(); // className
        tokenizer.skipNext(); // '{'

        Keyword keyword;
        // classVarDec
        tokenizer.lookupNext();
        while((keyword = JackTokenizer.keyword().orElse(Keyword.TRASH)) == Keyword.STATIC
                | keyword == Keyword.FIELD) {
            compileClassVarDec();
            tokenizer.lookupNext();
        }

        // subroutineDec
        tokenizer.lookupNext();
        while((keyword = JackTokenizer.keyword().orElse(Keyword.TRASH)) == Keyword.CONSTRUCTOR
                | keyword == Keyword.FUNCTION | keyword == Keyword.METHOD) {
            compileSubroutine();
            tokenizer.lookupNext();
        }
        tokenizer.skipNext(); // '}

        SymbolTable.init(); // SymbolTable 모두 초기화
    }

    void compileClassVarDec() {
        Kind kind = (tokenizer.returnNext().equals("static") ? Kind.STATIC : Kind.FIELD); // ('static' | 'field')
        String type = tokenizer.returnNext(); // type
        String name = tokenizer.returnNext(); // varName
        SymbolTable.define(name, type, kind);

        // (',' varName)*
        while(tokenizer.lookupNext().equals(",")) {
            tokenizer.skipNext(); // ','
            name = tokenizer.returnNext(); // varName
            SymbolTable.define(name, type, kind);
        }
        tokenizer.skipNext(); // ';'
    }

    void compileSubroutine() {
        curRange = Range.SUBROUTINE;
        SymbolTable.startSubroutine(); // 서브루틴 symbolTable 초기화

        boolean isConstructor = tokenizer.lookupNext().equals("constructor");
        boolean isMethod = tokenizer.lookupNext().equals("method");

        tokenizer.skipNext(); // ('constructor' | 'function' | 'method')
        tokenizer.skipNext(); // ('void' | type)
        subroutineName = tokenizer.returnNext(); // subroutineName
        tokenizer.skipNext(); // '('
        compileParameterList(); // parameterList
        tokenizer.skipNext(); // ')'

        // subroutineBody
        tokenizer.skipNext(); // '{'
        // varDec
        tokenizer.lookupNext();
        while(JackTokenizer.keyword().orElse(Keyword.TRASH) == Keyword.VAR) {
            compileVarDec();
            tokenizer.lookupNext();
        }
        // SymbolTable 모두 완성

        writer.writeFunction(className + "." + subroutineName, SymbolTable.varCount(Kind.VAR));

        if(isConstructor) {
            writer.writePush(Segment.CONST, SymbolTable.varCount(Kind.FIELD));
            writer.writeCall("Memory.alloc", 1);
            writer.writePop(Segment.POINTER, 0); // this 세그먼트의 시작 주소가 객체의 시작 주소가 되도록 설정
        } else if(isMethod) {
            writer.writePush(Segment.ARG, 0);
            writer.writePop(Segment.POINTER, 0);
        }

        compileStatements(); // statements
        tokenizer.skipNext(); // '}'
    }

    void compileParameterList() {
        String type, name;

        if(!tokenizer.lookupNext().equals(")")) {
            type = tokenizer.returnNext(); // type
            name = tokenizer.returnNext(); // varName
            SymbolTable.define(name, type, Kind.ARG);

            while (tokenizer.lookupNext().equals(",")) {
                tokenizer.skipNext(); // ','
                type = tokenizer.returnNext(); // type
                name = tokenizer.returnNext(); // varName
                SymbolTable.define(name, type, Kind.ARG);
            }
        }
    }

    void compileVarDec() {
        tokenizer.skipNext(); // 'var'

        String type = tokenizer.returnNext(); // type
        String name = tokenizer.returnNext(); // varName
        SymbolTable.define(name, type, Kind.VAR);

        while(tokenizer.lookupNext().equals(",")) {
            tokenizer.skipNext(); // ','
            name = tokenizer.returnNext(); // varName
            SymbolTable.define(name, type, Kind.VAR);
        }
        tokenizer.skipNext(); // ';'
    }

    void compileStatements() {
        while(true) {
            tokenizer.lookupNext();
            if(JackTokenizer.tokenType() != TokenType.KEYWORD) break;

            Keyword keyword = JackTokenizer.keyword().orElse(Keyword.TRASH);
            if(keyword == Keyword.LET) compileLet(); // letStatement
            else if(keyword == Keyword.IF) compileIf(labelIdx++); // ifStatement
            else if(keyword == Keyword.WHILE) compileWhile(labelIdx++); // whileStatement
            else if(keyword == Keyword.DO) compileDo(); // doStatement
            else if(keyword == Keyword.RETURN) compileReturn(); // returnStatement
            else break;
        }
    }

    void compileDo() {
        tokenizer.skipNext(); // 'do'

        String tmpClassName, tmpSubroutineName, tmp;
        int thisArg;

        tmpClassName = className;
        tmp = tokenizer.returnNext(); // subroutineName | (className | varName)

        // className | varName
        if(tokenizer.lookupNext().equals(".")) {
            tokenizer.skipNext(); // '.'
            // varName
            if(SymbolTable.kindOf(tmp) != Kind.NONE) {
                thisArg = 1;

                switch (SymbolTable.kindOf(tmp)) {
                    case STATIC:
                        writer.writePush(Segment.STATIC, SymbolTable.indexOf(tmp));
                        break;
                    case FIELD:
                        writer.writePush(Segment.THIS, SymbolTable.indexOf(tmp));
                        break;
                    case VAR:
                        writer.writePush(Segment.LOCAL, SymbolTable.indexOf(tmp));
                        break;
                    case ARG:
                        writer.writePush(Segment.ARG, SymbolTable.indexOf(tmp));
                        break;
                }
                tmpClassName = SymbolTable.typeOf(tmp); // 해당 변수의 클래스 타입(이름)
            }
            // className
            else {
                thisArg = 0;
                tmpClassName = tmp;
            }
            tmpSubroutineName = tokenizer.returnNext(); // subroutineName
        }
        // subroutineName
        else {
            thisArg = 1;
            writer.writePush(Segment.POINTER, 0);
            tmpSubroutineName = tmp;
        }

        tokenizer.skipNext(); // '('
        int nArgs = compileExpressionList();
        tokenizer.skipNext(); // ')'
        tokenizer.skipNext(); // ';'
        writer.writeCall(tmpClassName + "." + tmpSubroutineName, nArgs + thisArg);
    }

    void compileLet() {
        tokenizer.skipNext(); // 'let'
        String varName = tokenizer.returnNext(); // varName

        // 좌측 배열O
        if(tokenizer.lookupNext().equals("[")) {
            switch (SymbolTable.kindOf(varName)) {
                case STATIC:
                    writer.writePush(Segment.STATIC, SymbolTable.indexOf(varName));
                    break;
                case FIELD:
                    writer.writePush(Segment.THIS, SymbolTable.indexOf(varName));
                    break;
                case VAR:
                    writer.writePush(Segment.LOCAL, SymbolTable.indexOf(varName));
                    break;
                case ARG:
                    writer.writePush(Segment.ARG, SymbolTable.indexOf(varName));
                    break;
            }
            tokenizer.skipNext(); // '['
            compileExpression(); // expression -> 결과값 push 한 상태
            tokenizer.skipNext(); // ']'
            writer.writeArithmetic(Command.ADD);

            writer.writePop(Segment.TEMP, 7);
            tokenizer.skipNext(); // '='
            compileExpression(); // expression
            writer.writePush(Segment.TEMP, 7);
            writer.writePop(Segment.POINTER, 1);
            writer.writePop(Segment.THAT, 0);
        }
        // 좌측 배열X
        else {
            tokenizer.skipNext(); // '='
            compileExpression(); // expression
            switch (SymbolTable.kindOf(varName)) {
                case STATIC:
                    writer.writePop(Segment.STATIC, SymbolTable.indexOf(varName));
                    break;
                case FIELD:
                    writer.writePop(Segment.THIS, SymbolTable.indexOf(varName));
                    break;
                case VAR:
                    writer.writePop(Segment.LOCAL, SymbolTable.indexOf(varName));
                    break;
                case ARG:
                    writer.writePop(Segment.ARG, SymbolTable.indexOf(varName));
                    break;
            }
        }

        tokenizer.skipNext(); // ';'
    }

    void compileWhile(int labelIdx) {
        writer.writeLabel(subroutineName + ".while.L1." + labelIdx);
        tokenizer.skipNext(); // while
        tokenizer.skipNext(); // '('
        compileExpression(); // expression
        tokenizer.skipNext(); // ')'
        writer.writeArithmetic(Command.NOT); // false 이면 jump
        writer.writeIf(subroutineName + ".while.L2." + labelIdx);

        tokenizer.skipNext(); // '{'
        compileStatements(); // statements
        tokenizer.skipNext(); // '}'
        writer.writeGoto(subroutineName + ".while.L1." + labelIdx);
        writer.writeLabel(subroutineName + ".while.L2." + labelIdx);
    }

    void compileReturn() {
        tokenizer.skipNext(); // 'return'
        if(!tokenizer.lookupNext().equals(";")) compileExpression(); // expression
        else writer.writePush(Segment.CONST, 0);
        tokenizer.skipNext(); // ';'
        writer.writeReturn();
    }

    void compileIf(int labelIdx) {
        tokenizer.skipNext(); // 'if'
        tokenizer.skipNext(); // '('
        compileExpression(); // expression
        tokenizer.skipNext(); // ')'
        writer.writeArithmetic(Command.NOT); // false 이면 jump
        writer.writeIf(subroutineName + ".if.L2." + labelIdx);
        tokenizer.skipNext(); // '{'
        compileStatements(); // statements
        tokenizer.skipNext(); // '}'
        writer.writeGoto(subroutineName + ".if.L1." + labelIdx);

        writer.writeLabel(subroutineName + ".if.L2." + labelIdx);
        tokenizer.lookupNext();
        if(JackTokenizer.keyword().orElse(Keyword.TRASH) == Keyword.ELSE) {
            tokenizer.skipNext(); // 'else'
            tokenizer.skipNext(); // '{'
            compileStatements(); // statements
            tokenizer.skipNext(); // '}'
        }
        writer.writeLabel(subroutineName + ".if.L1." + labelIdx);
    }

    void compileExpression() {
        compileTerm(); // term
        while(tokenizer.lookupNext().matches("[+\\-*/&|<>=]")) {
            String op = tokenizer.returnNext(); // op
            compileTerm(); // term

            switch (op) {
                case "+": writer.writeArithmetic(Command.ADD); break;
                case "-": writer.writeArithmetic(Command.SUB); break;
                case "&": writer.writeArithmetic(Command.AND); break;
                case "|": writer.writeArithmetic(Command.OR); break;
                case "<": writer.writeArithmetic(Command.LT); break;
                case ">": writer.writeArithmetic(Command.GT); break;
                case "=": writer.writeArithmetic(Command.EQ); break;
                case "*":
                    writer.writeCall("Math.multiply", 2);
                    break;
                case "/":
                    writer.writeCall("Math.divide", 2);
                    break;
            }
        }
    }

    void compileTerm() {
        // '(' expression ')'
        if(tokenizer.lookupNext().equals("(")) {
            tokenizer.skipNext(); // '('
            compileExpression(); // expression
            tokenizer.skipNext(); // ')'
        }

        // unaryOp term
        else if(tokenizer.lookupNext().matches("[-~]")) {
            String op = tokenizer.returnNext(); // unaryOp
            compileTerm(); // term
            writer.writeArithmetic((op.equals("-")) ? Command.NEG : Command.NOT);
        }

        // IntegerConstant | stringConstant | keywordConstant | varName
        // varName '[' expression ']' | subroutineCall
        else {
            String tmp = tokenizer.returnNext();
            int nArgs;

            // varName | subroutineName | className
            switch (tokenizer.lookupNext()) {
                case "[": // tmp : varName
                    switch (SymbolTable.kindOf(tmp)) {
                        case STATIC:
                            writer.writePush(Segment.STATIC, SymbolTable.indexOf(tmp));
                            break;
                        case FIELD:
                            writer.writePush(Segment.THIS, SymbolTable.indexOf(tmp));
                            break;
                        case VAR:
                            writer.writePush(Segment.LOCAL, SymbolTable.indexOf(tmp));
                            break;
                        case ARG:
                            writer.writePush(Segment.ARG, SymbolTable.indexOf(tmp));
                            break;
                    }

                    tokenizer.skipNext(); // '['
                    compileExpression(); // expression
                    tokenizer.skipNext(); // ']'
                    writer.writeArithmetic(Command.ADD);
                    writer.writePop(Segment.POINTER, 1);
                    writer.writePush(Segment.THAT, 0);
                    return;
                case "(": // tmp : subroutineName
                    tokenizer.skipNext(); // '('
                    nArgs = compileExpressionList(); // expressionList
                    tokenizer.skipNext(); // ')'
                    writer.writePush(Segment.POINTER, 0);
                    writer.writeCall(className + "." + tmp, nArgs + 1);
                    return;
                case ".": // tmp : varName | className
                    tokenizer.skipNext(); // '.'
                    String tmpSubroutineName = tokenizer.returnNext(); // subroutineName

                    if(SymbolTable.kindOf(tmp) != Kind.NONE) {
                        switch (SymbolTable.kindOf(tmp)) {
                            case STATIC:
                                writer.writePush(Segment.STATIC, SymbolTable.indexOf(tmp));
                                break;
                            case FIELD:
                                writer.writePush(Segment.THIS, SymbolTable.indexOf(tmp));
                                break;
                            case VAR:
                                writer.writePush(Segment.LOCAL, SymbolTable.indexOf(tmp));
                                break;
                            case ARG:
                                writer.writePush(Segment.ARG, SymbolTable.indexOf(tmp));
                                break;
                        }
                    }

                    tokenizer.skipNext(); // '('
                    nArgs = compileExpressionList(); // expressionList
                    tokenizer.skipNext(); // ')'

                    if(SymbolTable.kindOf(tmp) != Kind.NONE) {
                        writer.writeCall(SymbolTable.typeOf(tmp) + "." + tmpSubroutineName, nArgs + 1); // method
                    } else {
                        writer.writeCall(tmp + "." + tmpSubroutineName, nArgs); // function
                    }
                    return;
            }

            int index;
            // IntegerConstant | stringConstant | keywordConstant | varName
            if(tmp.matches("[0-9]+")) {
                writer.writePush(Segment.CONST, Integer.parseInt(tmp));
            } else if(tmp.equals("true")) {
                writer.writePush(Segment.CONST, 1);
                writer.writeArithmetic(Command.NEG);
            } else if(tmp.equals("false") || tmp.equals("null")) {
                writer.writePush(Segment.CONST, 0);
            } else if(tmp.equals("this")) {
                writer.writePush(Segment.POINTER, 0);
            } else if((index = SymbolTable.indexOf(tmp)) != -1) {
                switch (SymbolTable.kindOf(tmp)) {
                    case STATIC:
                        writer.writePush(Segment.STATIC, index);
                        break;
                    case FIELD:
                        writer.writePush(Segment.THIS, index);
                        break;
                    case VAR:
                        writer.writePush(Segment.LOCAL, index);
                        break;
                    case ARG:
                        writer.writePush(Segment.ARG, index);
                        break;
                }
            }
            else { // stringConstant
                writer.writePush(Segment.CONST, tmp.length());
                writer.writeCall("String.new", 1); // String 의 사작주소 스택에 push

                for(int i = 0; i < tmp.length(); i++) {
                    writer.writePush(Segment.CONST, tmp.charAt(i));
                    writer.writeCall("String.appendChar", 2); // 새로 생성된 String 의 시작주소 스택에 push
                }
            }
        }
    }

    int compileExpressionList() {
        int nArgs = 0;

        if(!tokenizer.lookupNext().equals(")")) {
            compileExpression(); // expression
            nArgs += 1;
            while (tokenizer.lookupNext().equals(",")) {
                tokenizer.skipNext(); // ','
                compileExpression(); // expression
                nArgs += 1;
            }
        }
        return nArgs;
    }
}
