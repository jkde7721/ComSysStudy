import sys, os, os.path, glob
import jacktokenizer
import compilationengine
class JackAnalyzer:
    def __init__(self):
        pass

    def check(self, inputfiles, outputfile):
        for inputfile in inputfiles:
            #jacktokenizer.tokenizer(inputfile)
            #tokenizer = jacktokenizer.JackTokenizer(inputfile)
            #print(tokenizer.tokens)
            compiler = compilationengine.CompilationEngine(inputfile, outputfile)


if sys.argv[1].endswith('.jack'): # 하나의 파일
    inputfiles = [sys.argv[1]]
    outputfile = sys.argv[1].replace('.jack','.xml') # 파일명으로 파일 생성
else: # 디렉터리명
    inputfiles = glob.glob(sys.argv[1]+'/*.jack')
    outputfile = sys.argv[1]+'/'+sys.argv[1]+'.xml' # 디렉터리명으로 파일 생성

analyzer = JackAnalyzer()
analyzer.check(inputfiles, outputfile)
