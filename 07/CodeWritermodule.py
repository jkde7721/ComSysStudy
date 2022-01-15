import Parsermodule

class CodeWriter:
    asmCommands = list()
    labelNum = 0

    def __init__(self, outfile, filename, commands):
        self.commands = commands
        self.f = open(outfile, 'w')

    def setFileName(self, filename):
        self.filename = filename.split("/")[-1]
        name_ext = self.filename.split(".")
        self.filename_without_ext = name_ext[0]
        self.ext = "."+name_ext[1]
        self.parser = Parsermodule.Parser(filename)

    def close(self):
        self.f.close()

    def codewrite(self):
        for command in self.commands:
            for arg in command:
                #print("arg" + arg)
                if arg in self.parser.command_type:
                    command_type = self.parser.command_type.get(arg)
            if command_type == self.parser.C_ARITHMETIC:
                self.writerArithmatic(command)
            elif command_type == self.parser.C_POP or command_type == self.parser.C_PUSH:
                self.writePushPop(command, self.parser.arg1(command), int(self.parser.arg2(command)))
        self.asmCommands.append("(END)")
        self.asmCommands.append("@END")
        self.asmCommands.append("0;JMP")

        for command in self.asmCommands:
            #print(command)
            self.f.write(command+"\n")

        self.close()

    # 참고했음
    def writerArithmatic(self, command):
        arg1 = self.parser.arg1(command)
        if arg1 in ['neg', 'not']:
            self.writePushPop("pop temp 0", 'temp', 0)
            self.asmCommands.append("@R5")
            if arg1 == "not":
                self.asmCommands.append("M=!M")
            elif arg1 == "neg":
                self.asmCommands.append("M=-M")
        else:
            self.writePushPop("pop temp 0", "temp", 0)
            self.writePushPop("pop temp 1", "temp", 1)
            self.asmCommands.append("@R6")
            self.asmCommands.append("D=M")
            self.asmCommands.append("@R5")
            op=''
            if arg1 in ['add', 'sub', 'and', 'or']:
                if arg1 == "add":
                    op = '+'
                elif arg1 == "sub":
                    op = '-'
                elif arg1 == 'and':
                    op = '&'
                elif arg1 == 'or':
                    op = '|'
                self.asmCommands.append("M=D"+op+"M")

            elif arg1 in ['eq', 'gt', 'lt']:
                self.asmCommands.append("D=D-M")
                self.asmCommands.append("@TRUE"+str(self.labelNum))
                jump=''
                if arg1 == "eq":
                    jump = 'JEQ'
                elif arg1 == 'gt':
                    jump = 'JGT'
                elif arg1 == 'lt':
                    jump = 'JLT'
                self.asmCommands.append("D;"+jump)
                self.asmCommands.append("@R5")
                self.asmCommands.append("M=0")
                self.asmCommands.append("@PUSH" + str(self.labelNum))
                self.asmCommands.append("0;JMP")
                self.asmCommands.append("(TRUE" + str(self.labelNum) + ")")
                self.asmCommands.append("@R5")
                self.asmCommands.append("M=-1")
                self.asmCommands.append("(PUSH" + str(self.labelNum) + ")")
                self.labelNum += 1
            self.writePushPop("push temp 0", "temp", 0)

    def writePushPop(self, command, segment, index):
        command_type = ""
        for cmd in command:
            if cmd in self.parser.command_type:
                command_type = self.parser.command_type.get(cmd)
        #print("command type : ", command_type)
        seg1 = ['local','argument','this','that']
        seg2 = ['pointer', 'temp']
        seg3 = 'constant'
        seg4 = 'static'
        ram = ""
        if segment == "local":
            ram = 'LCL'
        elif segment == "argument":
            ram = 'ARG'
        elif segment == "this":
            ram = 'THIS'
        elif segment == "pointer":
            ram = 'THIS'
        elif segment == "that":
            ram = 'THAT'
        elif segment == "temp":
            ram = 'R5'

        cmd = ""
        if segment == "pointer":
            cmd = 'D=A'
        elif segment == "temp":
            cmd = 'D=M'

        if command_type == self.parser.C_PUSH:
            if (segment in seg1) or (segment in seg2) :
                self.asmCommands.append("@"+ram)
                self.asmCommands.append(cmd)
                self.asmCommands.append("@"+str(index))
                self.asmCommands.append("A=D+A")
                self.asmCommands.append("D=M")

            else:
                if segment == seg3:
                    self.asmCommands.append("@"+str(index))
                    self.asmCommands.append("D=A")
                else:
                    self.asmCommands.append("@"+self.filename+"."+str(index))
                    self.asmCommands.append("D=M")
            self.asmCommands.append("@SP")
            self.asmCommands.append("M=M+1")
            self.asmCommands.append("A=M-1")
            self.asmCommands.append("M=D")
        elif command_type == self.parser.C_POP:
            if (segment in seg1) or (segment in seg2) :
                self.asmCommands.append("@SP")
                self.asmCommands.append("M=M-1")
                self.asmCommands.append("A=M")
                self.asmCommands.append("D=M")
                self.asmCommands.append("@R13")
                self.asmCommands.append("M=D")
                self.asmCommands.append("@" + ram)
                self.asmCommands.append(cmd)
                self.asmCommands.append("@" + str(index))
                self.asmCommands.append("D=D+A")
                self.asmCommands.append("@R14")
                self.asmCommands.append("M=D")
                self.asmCommands.append("@R13")
                self.asmCommands.append("D=M")
                self.asmCommands.append("@R14")
                self.asmCommands.append("A=M")
                self.asmCommands.append("M=D")
            elif segment == seg4:
                self.asmCommands.append("@SP")
                self.asmCommands.append("M=M-1")
                self.asmCommands.append("A=M")
                self.asmCommands.append("D=M")
                self.asmCommands.append("@" + self.filename+ + "." + str(index))
                self.asmCommands.append("M=D")
