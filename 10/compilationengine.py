from constant import *
import jacktokenizer

class CompilationEngine:
    def __init__(self, inputfile, outputfile):
        self.tab=''
        self.tokenizer = jacktokenizer.JackTokenizer(inputfile)
        self.startoutputfile(outputfile)
        self.compileclass()
        self.finishoutputfile()
        self.tokenizer.fileclose()
        

    def startoutputfile(self, outputfile):
        self.outputfile = open(outputfile, 'a')
        self.outputfile.write('<tokens>\n') 

    def finishoutputfile(self):
        self.outputfile.write('</tokens>')
        self.outputfile.close()
    
    def writeterminal(self):
        if(self.tokenizer.hasMoreTokens()):
            self.tokenizer.advance()
            #type = self.tokenizer.cur_token_type
            val = self.tokenizer.cur_token_val

            if val == '<':
                val = '&lt;'
            elif val == '>':
                val = '&gt;'
            elif val == '\"':
                val = '&quot;'
            elif val == '&':
                val = '&amp;'
            
                
            #print(self.tab+'<'+tokens[self.tokenizer.tokenType(self.tokenizer.cur_token_val)]+'> '+val+' </'+tokens[self.tokenizer.tokenType(self.tokenizer.cur_token_val)]+'>\n')
            self.outputfile.write(self.tab+'<'+tokens[self.tokenizer.tokenType(self.tokenizer.cur_token_val)]+'> '+val+' </'+tokens[self.tokenizer.tokenType(self.tokenizer.cur_token_val)]+'>\n')

    def startnonterminal(self, type):
        
        self.outputfile.write(self.tab+'<'+ type +'>\n')
        self.tab += '  '

    def finishnonterminal(self, type):
        self.tab = self.tab[:-2]
        self.outputfile.write(self.tab+'</'+ type +'>\n')
        

    def compileclass(self):
        self.startnonterminal('class')
        self.writeterminal() # 'class'
        self.writeterminal() # className
        self.writeterminal() # '{'
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            
            while self.tokenizer.tokens[0][0] == T_KEYWORD and self.tokenizer.tokens[0][1] == KW_STATIC or self.tokenizer.tokens[0][1] == KW_FIELD :
                #print(self.tokenizer.tokens[0][1])
                self.compileclassvardec()

        if self.tokenizer.hasMoreTokens():
            # type = self.tokenizer.tokens[0][0]
            # val = self.tokenizer.tokens[0][1]
            while self.tokenizer.tokens[0][0] == T_KEYWORD and (self.tokenizer.tokens[0][1] == KW_CONSTRUCTOR or self.tokenizer.tokens[0][1] == KW_FUNCTION or self.tokenizer.tokens[0][1] == KW_METHOD):
                self.compilesubroutine()
        self.writeterminal() # '}'
        self.finishnonterminal('class')

    def compileclassvardec(self):
        self.startnonterminal('classVarDec')
        self.writeterminal() # ('static'|'field')
        self.writeterminal() # type
        self.writeterminal() # varName

        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
        
            while self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == ',':
                self.writeterminal() # ','
                self.writeterminal() # 'varname'
        self.writeterminal() # ';'

        self.finishnonterminal('classVarDec')
        
    def compilesubroutine(self):
        self.startnonterminal('subroutineDec')
        self.writeterminal() # ('constructor'|'function'|'method')
        self.writeterminal() # ('void'|type)
        self.writeterminal() # subroutineName
        self.writeterminal() # '('
        self.compileparameterlist() # parameterList
        self.writeterminal() # ')'
        
        self.compilesubroutinebody()
        self.finishnonterminal('subroutineDec')

    def compilesubroutinebody(self):
        self.startnonterminal('subroutineBody')
        self.writeterminal() # '{'
        # varDec*
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            while self.tokenizer.tokens[0][0] == T_KEYWORD and self.tokenizer.tokens[0][1] == KW_VAR:
                self.compilevardec()
        # statements
        self.compilestatements()
        self.writeterminal() # '}'
        self.finishnonterminal('subroutineBody')

    def compileparameterlist(self):
        self.startnonterminal('parameterList')

        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]

            while self.tokenizer.tokens[0][0] == T_KEYWORD and (self.tokenizer.tokens[0][1] in [KW_BOOLEAN, KW_CHAR, KW_INT]):
                self.writeterminal() # type
                self.writeterminal() # varName
                if self.tokenizer.hasMoreTokens():
                    type = self.tokenizer.tokens[0][0]
                    val = self.tokenizer.tokens[0][1]
                    if type == T_SYM and val == ',':
                        self.writeterminal() # ','
                    else:
                        break
        
        self.finishnonterminal('parameterList')
        
    def compilevardec(self):
        self.startnonterminal('varDec')
        self.writeterminal() # 'var'
        self.writeterminal() # type
        self.writeterminal() # varName
        
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            while self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == ',':
                self.writeterminal() # ','
                self.writeterminal() # varName

        self.writeterminal() # ';'
        
        self.finishnonterminal('varDec')
    
    def compilestatements(self):
        self.startnonterminal('statements')
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            while self.tokenizer.tokens[0][0] == T_KEYWORD and self.tokenizer.tokens[0][1] in [KW_LET, KW_IF, KW_WHILE, KW_DO, KW_RETURN]:
                if self.tokenizer.tokens[0][1] == KW_LET:
                    self.compilelet()
                elif self.tokenizer.tokens[0][1] == KW_IF:
                    self.compileif()
                elif self.tokenizer.tokens[0][1] == KW_WHILE:
                    self.compilewhile()
                elif self.tokenizer.tokens[0][1] == KW_DO:
                    self.compiledo()
                elif self.tokenizer.tokens[0][1] == KW_RETURN:
                    self.compilereturn()
        self.finishnonterminal('statements')


    def compiledo(self):
        self.startnonterminal('doStatement')
        self.writeterminal() # 'do'

        # subroutinCall
        self.writeterminal() # subroutinName | (className | varName)
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            if self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == '.': # (className | varName) 였던 경우
                self.writeterminal() # '.'
                self.writeterminal() # subroutineName
        self.writeterminal() # '('
        self.compileexpressionlist()
        self.writeterminal() # ')'
        self.writeterminal() # ';'
        
        self.finishnonterminal('doStatement')

   
    def compilelet(self):
        self.startnonterminal('letStatement')
        self.writeterminal() # 'let'
        self.writeterminal() # varName
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            if self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == '[':
                self.writeterminal() # '['
                self.compileexpression()
                self.writeterminal() # ']'
        self.writeterminal() # '='
        self.compileexpression()
        self.writeterminal() # ';'
        self.finishnonterminal('letStatement')

    def compilewhile(self):
        self.startnonterminal('whileStatement')
        self.writeterminal() # 'while'
        self.writeterminal() # '('
        self.compileexpression()
        self.writeterminal() # ')'
        self.writeterminal() # '{'
        self.compilestatements()
        self.writeterminal() # '}'
        self.finishnonterminal('whileStatement')

    def compilereturn(self):
        self.startnonterminal('ReturnStatement')
        self.writeterminal() # 'return'

        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            if self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == ';':
                self.writeterminal() # ';'
            else:
                self.compileexpression()
                self.writeterminal() # ';'

        self.finishnonterminal('ReturnStatement')

    def compileif(self):
        self.startnonterminal('ifStatement')
        self.writeterminal() # 'if'
        self.writeterminal() # '('
        self.compileexpression()
        self.writeterminal() # ')'
        self.writeterminal() # '{'
        self.compilestatements()
        self.writeterminal() # '}'

        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            if self.tokenizer.tokens[0][0] == T_KEYWORD and self.tokenizer.tokens[0][1] == KW_ELSE:
                self.writeterminal() # 'else'
                self.writeterminal() # '{'
                self.compilestatements()
                self.writeterminal() # '}'

        self.finishnonterminal('ifStatement')

    def compileexpression(self):
        self.startnonterminal('expression')
        
        self.compileterm()
        
        if self.tokenizer.hasMoreTokens():
            #type = self.tokenizer.tokens[0][0]
            #val = self.tokenizer.tokens[0][1]
            while self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] in ['+','-','*','/','&','|','<','>','=']:
                self.writeterminal() # op
                self.compileterm()

        self.finishnonterminal('expression')

    def compileterm(self):
        self.startnonterminal('term')
        if self.tokenizer.hasMoreTokens():
            type = self.tokenizer.tokens[0][0]
            val = self.tokenizer.tokens[0][1]
            if type == T_NUM or type == T_STR or (type == T_KEYWORD and val in [KW_TRUE, KW_FALSE, KW_NULL, KW_THIS]):
                self.writeterminal() # constant
            elif type == T_SYM and val == '(':
                self.writeterminal() # '('
                self.compileexpression()
                type = self.tokenizer.tokens[0][0]
                val = self.tokenizer.tokens[0][1]
                if type == T_SYM and val == ')':
                    self.writeterminal() # ')'
            elif type == T_SYM and val in ['-','~']:
                self.writeterminal() # unaryOp
                self.compileterm()
            else:
                self.writeterminal() # varName | subroutineName | className | varName | varName
                if self.tokenizer.hasMoreTokens():
                    type = self.tokenizer.tokens[0][0]
                    val = self.tokenizer.tokens[0][1]
                    if type == T_SYM and val == '[':
                        self.writeterminal() # '['
                        self.compileexpression()
                        self.writeterminal() # ']'
                    elif type == T_SYM and val == '(':
                        self.writeterminal() # '('
                        self.compileexpressionlist()
                        self.writeterminal() # ')'
                    elif type == T_SYM and val == '.':
                        self.writeterminal() # '.'
                        self.writeterminal() # subroutineName
                        self.writeterminal() # '('
                        self.compileexpressionlist()
                        self.writeterminal() # ')'

        self.finishnonterminal('term')

    def compileexpressionlist(self):
        self.startnonterminal('expressionList')
        
        
        
        if self.tokenizer.hasMoreTokens():
            type = self.tokenizer.tokens[0][0]
            val = self.tokenizer.tokens[0][1]
            if val != ')':
                self.compileexpression()
                while self.tokenizer.tokens[0][0] == T_SYM and self.tokenizer.tokens[0][1] == ',':
                    self.writeterminal() # ','
                    self.compileexpression()

        self.finishnonterminal('expressionList')