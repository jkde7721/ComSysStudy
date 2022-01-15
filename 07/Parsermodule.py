import re


class Parser:
    C_ARITHMETIC = 0
    C_PUSH       = 1
    C_POP        = 2
    C_LABEL      = 3
    C_GOTO       = 4
    C_IF         = 5
    C_FUNCTION   = 6
    C_RETURN     = 7
    C_CALL       = 8
    C_ERROR      = 9

    def __init__(self, file_name):
        file = open(file_name, 'r')
        self.lines = file.read()
        self.lines = self.lines.split('\n')
        self.lines = [self._remove_comment(i) for i in self.lines if self._remove_comment(i) != '']
        self.tokens = [t.split() for t in self.lines]
        self.cur_command = self.tokens[0]
        #print(self.tokens)

        self.command_type = {'add':self.C_ARITHMETIC, 'sub':self.C_ARITHMETIC, 'neg':self.C_ARITHMETIC,
                     'eq' :self.C_ARITHMETIC, 'gt' :self.C_ARITHMETIC, 'lt' :self.C_ARITHMETIC,
                     'and':self.C_ARITHMETIC, 'or' :self.C_ARITHMETIC, 'not':self.C_ARITHMETIC,
                     'label':self.C_LABEL,    'goto':self.C_GOTO,      'if-goto':self.C_IF,
                     'push':self.C_PUSH,      'pop':self.C_POP, 'call':self.C_CALL,      'return':self.C_RETURN,  'function':self.C_FUNCTION}

    _comment = re.compile('//.*$') # // 뒤에 임의의 문자 0번 이상 반복하고 끝
    def _remove_comment(self, line):
        return self._comment.sub('', line) #주석 삭제

    def _remove_space(self, line):
        return line.replace(" ", "")

    def hasMoreCommands(self):
        return len(self.tokens) > 0

    def advance(self):
        if self.hasMoreCommands():
            del self.tokens[0]
            #print(self.tokens)
            self.cur_command = self.tokens[0]
            #print(self.cur_command)

    def arg1(self, command):
        i = self.command_type[command[0]]
        #print("command",command)
        try:
            if i == self.C_ARITHMETIC:
                #print("0 : ",command[0])
                return command[0]
            else:
                #print("1 : ",command[1])
                return command[1]
        except:
            print("arg1 with C_RETURN")
            return ""

    def arg2(self, command):
        i = self.command_type[command[0]]
        if i in [self.C_PUSH,self.C_POP,self.C_FUNCTION,self.C_CALL]:
            #print(command[2])
            return command[2]
"""
parser = Parser("D:\StackTest.vm")
parser.arg1(parser.cur_command)
parser.arg2(parser.cur_command)
parser.advance()
"""
