#
import re
#
#NUM     = 1     # number e.g. '123'
#ID      = 2     # symbol e.g. 'LOOP'
#OP      = 3     # = ; ( ) @ + - & | !
#ERROR   = 4     # error in file

class Parser:

    def __init__(self, fileName):
        self.file = open(fileName, 'r')
        #self._tokens =
        #self._tokens = [t for t in [self._tokenize_line(l) for l in self._tokens] if t!=[]]
        #self.cur_command = []        # list of tokens for current command
        #self.cur_token = (ERROR,0)   # current token of current command
    def commandType(self,line):
        if '@' in line:
            type = 'A_COMMAND'
        elif '(' in line:
            type = 'L_COMMAND'
        else:
            type = 'C_COMMAND'
        return type

    def symbol(self, type, line):
        if type == 'A_COMMAND':
            line = line.split('@')[1]
        elif type == 'L_COMMAND':
            line = line.split('(')[1].split(')')[0]
        return line

    def dest_comp_jump(self, line):
        # return dest, comp, jump
        if "=" in line and ";" in line: # dest=comp;jump
            return line.split("=")[0], line.split("=")[1].split(";")[0], line.split(";")[1]
        elif "=" in line: # dest=comp
            return line.split("=")[0], line.split("=")[1], "000"
        elif ";" in line: # comp;jump
            return "000",line.split(";")[0],line.split(";")[1]
        else: #comp
            return "000", line, "000"

    _comment = re.compile('//.*$')
    def _remove_comment(self, line):
        return self._comment.sub('', line) #주석 삭제

    def _remove_space(self, line):
        return line.replace(" ", "")

    #def hasMoreCommands(self):
    #    return
    #def _tokenize_line(self, line):
    #    return [self._token(word) for word in self._split(self._remove_comment(line))]

"""
parser = Parser("D:/Max.asm")


_lines = parser.file.read()
_tokens = _lines.split('\n') # 줄바꿈 단위로 list element


for l in _tokens:
    l = parser._remove_comment(l)
    l = parser._remove_space(l)
    print(l)
    if len(l) > 0:
        type = parser.commandType(l)
        if type != "C_COMMAND":
            l = parser.symbol(type, l)
        else:
            l_dest, l_comp, l_jump = parser.dest_comp_jump(l)

        print(l)
        print()
"""
