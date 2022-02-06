package comsys.compiler;

import java.io.*;

public class CompilationEngine {

    BufferedWriter bufferedWriter;
    JackTokenizer tokenizer;
    String tab = "";

    public CompilationEngine(JackTokenizer tokenizer, String filePath) {
        try {
            this.tokenizer = tokenizer;
            bufferedWriter = new BufferedWriter(new FileWriter(filePath));
        } catch (Exception e) {}
    }

    public void compile() throws IOException {
        compileClass();
        bufferedWriter.close();
    }

    void compileClass() {
        writeOpenTag("class");

        writeTerminal(); // 'class'
        writeTerminal(); // className
        writeTerminal(); // '{'

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
        writeTerminal(); // '}

        writeCloseTag("class");
    }

    void compileClassVarDec() {
        writeOpenTag("classVarDec");

        writeTerminal(); // ('static' | 'field')
        writeTerminal(); // type
        writeTerminal(); // varName

        // (',' varName)*
        while(tokenizer.lookupNext().equals(",")) {
            writeTerminal(); // ','
            writeTerminal(); // varName
        }
        writeTerminal(); // ';'

        writeCloseTag("classVarDec");
    }

    void compileSubroutine() {
        writeOpenTag("subroutineDec");

        writeTerminal(); // ('constructor' | 'function' | 'method')
        writeTerminal(); // ('void' | type)
        writeTerminal(); // subroutineName
        writeTerminal(); // '('
        compileParameterList(); // parameterList
        writeTerminal(); // ')'

        // subroutineBody
        writeOpenTag("subroutineBody");
        writeTerminal(); // '{'
        // varDec
        tokenizer.lookupNext();
        while(JackTokenizer.keyword().orElse(Keyword.TRASH) == Keyword.VAR) {
            compileVarDec();
            tokenizer.lookupNext();
        }
        compileStatements(); // statements
        writeTerminal(); // '}'
        writeCloseTag("subroutineBody");

        writeCloseTag("subroutineDec");
    }

    void compileParameterList() {
        writeOpenTag("parameterList");

        if(!tokenizer.lookupNext().equals(")")) {
            writeTerminal(); // type
            writeTerminal(); // varName

            while (tokenizer.lookupNext().equals(",")) {
                writeTerminal(); // ','
                writeTerminal(); // type
                writeTerminal(); // varName
            }
        }

        writeCloseTag("parameterList");
    }

    void compileVarDec() {
        writeOpenTag("varDec");

        writeTerminal(); // 'var'
        writeTerminal(); // type
        writeTerminal(); // varName

        while(tokenizer.lookupNext().equals(",")) {
            writeTerminal(); // ','
            writeTerminal(); // varName
        }
        writeTerminal(); // ';'

        writeCloseTag("varDec");
    }

    void compileStatements() {
        writeOpenTag("statements");

        while(true) {
            tokenizer.lookupNext();
            if(JackTokenizer.tokenType() != TokenType.KEYWORD) break;

            Keyword keyword = JackTokenizer.keyword().orElse(Keyword.TRASH);
            if(keyword == Keyword.LET) compileLet(); // letStatement
            else if(keyword == Keyword.IF) compileIf(); // ifStatement
            else if(keyword == Keyword.WHILE) compileWhile(); // whileStatement
            else if(keyword == Keyword.DO) compileDo(); // doStatement
            else if(keyword == Keyword.RETURN) compileReturn(); // returnStatement
            else break;
        }

        writeCloseTag("statements");
    }

    void compileDo() {
        writeOpenTag("doStatement");

        writeTerminal(); // 'do'

        // subroutineCall
        writeTerminal(); // subroutineName | (className | varName)
        if(tokenizer.lookupNext().equals(".")) {
            writeTerminal(); // '.'
            writeTerminal(); // subroutineName
        }

        writeTerminal(); // '('
        compileExpressionList(); // expressionList
        writeTerminal(); // ')'
        writeTerminal(); // ';'

        writeCloseTag("doStatement");
    }

    void compileLet() {
        writeOpenTag("letStatement");

        writeTerminal(); // 'let'
        writeTerminal(); // varName

        if(tokenizer.lookupNext().equals("[")) {
            writeTerminal(); // '['
            compileExpression(); // expression
            writeTerminal(); // ']'
        }

        writeTerminal(); // '='
        compileExpression(); // expression
        writeTerminal(); // ';'

        writeCloseTag("letStatement");
    }

    void compileWhile() {
        writeOpenTag("whileStatement");

        writeTerminal(); // while
        writeTerminal(); // '('
        compileExpression(); // expression
        writeTerminal(); // ')'

        writeTerminal(); // '{'
        compileStatements(); // statements
        writeTerminal(); // '}'

        writeCloseTag("whileStatement");
    }

    void compileReturn() {
        writeOpenTag("returnStatement");

        writeTerminal(); // 'return'
        if(!tokenizer.lookupNext().equals(";")) compileExpression(); // expression
        writeTerminal(); // ';'

        writeCloseTag("returnStatement");
    }

    void compileIf() {
        writeOpenTag("ifStatement");

        writeTerminal(); // 'if'
        writeTerminal(); // '('
        compileExpression(); // expression
        writeTerminal(); // ')'
        writeTerminal(); // '{'
        compileStatements(); // statements
        writeTerminal(); // '}'

        tokenizer.lookupNext();
        if(JackTokenizer.keyword().orElse(Keyword.TRASH) == Keyword.ELSE) {
            writeTerminal(); // 'else'
            writeTerminal(); // '{'
            compileStatements(); // statements
            writeTerminal(); // '}'
        }

        writeCloseTag("ifStatement");
    }

    void compileExpression() {
        writeOpenTag("expression");

        compileTerm(); // term
        while(tokenizer.lookupNext().matches("[+\\-*/&|<>=]")) {
            writeTerminal(); // op
            compileTerm(); // term
        }

        writeCloseTag("expression");
    }

    void compileTerm() {
        writeOpenTag("term");

        // '(' expression ')'
        if(tokenizer.lookupNext().equals("(")) {
            writeTerminal(); // '('
            compileExpression(); // expression
            writeTerminal(); // ')'
        }
        // unaryOp term
        else if(tokenizer.lookupNext().matches("[-~]")) {
            writeTerminal(); // unaryOp
            compileTerm(); // term
        }
        // IntegerConstant | stringConstant | keywordConstant | varName
        // varName '[' expression ']' | subroutineCall
        else {
            writeTerminal(); // IntegerConstant | stringConstant | keywordConstant | varName | subroutineName | className
            switch (tokenizer.lookupNext()) {
                case "[":
                    writeTerminal(); // '['
                    compileExpression(); // expression
                    writeTerminal(); // ']'
                    break;
                case "(":
                    writeTerminal(); // '('
                    compileExpressionList(); // expressionList
                    writeTerminal(); // ')'
                    break;
                case ".":
                    writeTerminal(); // '.'
                    writeTerminal(); // subroutineName
                    writeTerminal(); // '('
                    compileExpressionList(); // expressionList
                    writeTerminal(); // ')'
                    break;
            }
        }

        writeCloseTag("term");
    }

    void compileExpressionList() {
        writeOpenTag("expressionList");

        if(!tokenizer.lookupNext().equals(")")) {
            compileExpression(); // expression
            while (tokenizer.lookupNext().equals(",")) {
                writeTerminal(); // ','
                compileExpression(); // expression
            }
        }

        writeCloseTag("expressionList");
    }

    void writeFile(String str) {
        try {
            bufferedWriter.write(str + "\n");
            bufferedWriter.flush();
        } catch (Exception e) {}
    }

    void writeOpenTag(String tagName) {
        writeFile(tab + "<" + tagName + ">");
        tab += "  ";
    }

    void writeCloseTag(String tagName) {
        tab = tab.substring(2);
        writeFile(tab + "</" + tagName + ">");
    }

    void writeTerminal() {
        if(tokenizer.hasMoreTokens()) {
            tokenizer.advance();
            switch (JackTokenizer.tokenType()) {
                case KEYWORD:
                    writeFile(tab + "<keyword> " + JackTokenizer.keyword().orElse(Keyword.TRASH).getValue() + " </keyword>");
                    break;
                case SYMBOL:
                    writeFile(tab + "<symbol> " + JackTokenizer.symbol() + " </symbol>");
                    break;
                case INT_CONST:
                    writeFile(tab + "<integerConstant> " + JackTokenizer.intVal() + " </integerConstant>");
                    break;
                case STRING_CONST:
                    writeFile(tab + "<stringConstant> " + JackTokenizer.stringVal() + " </stringConstant>");
                    break;
                case IDENTIFIER:
                    writeFile(tab + "<identifier> " + JackTokenizer.identifier() + " </identifier>");
                    break;
            }
        }
    }
}
