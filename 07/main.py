import Parsermodule
import CodeWritermodule

parser = Parsermodule.Parser("D:\PointerTest.vm")
codewriter = CodeWritermodule.CodeWriter("D:\PointerTest.asm", "D:\PointerTest.vm", parser.tokens)
codewriter.setFileName("D:\PointerTest.vm")
codewriter.codewrite()
