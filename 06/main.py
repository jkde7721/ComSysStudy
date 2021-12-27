import parsermodule
import symbol_tablemodule
import codemodule


"""
1패스
기호테이블만 구성
현재 명령이 로드될 ROM 주소 기록하는 숫자 : 0에서 시작,
                                          C-명령어나 A-명령어 등장할 때마다 1씩 증가,
                                          레이블 의사명령(Xxx)이나 주석 등장할 땐 증가 안 함,
                                          의사명령(Xxx) 나올 때 다음 명령 저장할 ROM 주소와 Xxx 연결하는 값 기호 테이블에 입력
"""
#입력하고 돌리기
filename = "D:/Max.asm"

parser1 = parsermodule.Parser(filename)
symbolTable = symbol_tablemodule.Symol_table()

rom_num = 0

_lines = parser1.file.read()
_tokens = _lines.split('\n') # 줄바꿈 단위로 list element

for l in _tokens:
    l = parser1._remove_comment(l)
    l = parser1._remove_space(l)
    #print(l)
    if len(l) > 0:
        type = parser1.commandType(l)
        if type != "L_COMMAND": # A나 C일 때
            rom_num = rom_num + 1

        else:                   # L일 때
            symbolTable.addEntry(parser1.symbol(type=type,line=l),rom_num)

parser1.file.close()

"""
2패스
다시 프로그램 스캔
기호가 있는 A-명령어 만날 때마다(@Xxx에서 Xxx가 숫자 아닌 기호일 때) 테이블에서 Xxx 조회
    테이블에 기호 있으면 기호에 대응되는 숫자 값으로 교체하여 해당 명령 번역 완료
    기호가 테이블에 없다면 새로운 변수, 기호 테이블에 (Xxx, n) 쌍 추가, n은 가용한 RAM 주소로 16에서 시작하여 1씩 증가

"""
parser2 = parsermodule.Parser(filename)
coder = codemodule.Code()

ram_num=16
outfilename = filename[:-3]+"hack"
f = open(outfilename,'w')


_lines = parser2.file.read()
_tokens = _lines.split('\n') # 줄바꿈 단위로 list element


for l in _tokens:
    l = parser2._remove_comment(l)
    l = parser2._remove_space(l)

    if len(l) > 0:
        type = parser2.commandType(l)
        if type == "A_COMMAND":
            symbol = parser2.symbol(type, l)
            if symbol.isdigit(): # 모두 숫자
                f.write(str(bin(symbol))[2:].zfill(16))
            else: # 기호
                if symbolTable.contains(symbol):
                    address = symbolTable.GetAddress(symbol)
                    f.write(str(bin(address)[2:]).zfill(16))
                else:
                    symbolTable.addEntry(symbol, ram_num)
                    f.write(str(bin(ram_num)[2:]).zfill(16))
                    ram_num = ram_num+1
            f.write("\n")
        elif type == "C_COMMAND":
            f.write("111")
            print("l : "+l)
            dest, comp, jump = parser2.dest_comp_jump(l)
            print("dest : " + dest)
            print("comp : " +comp)
            print("jump : " +jump)
            try:
                f.write(coder.comp(comp))
            except KeyError:
                f.write(comp)

            try:
                f.write(coder.dest(dest))
            except KeyError:
                f.write(dest)
            try:
                f.write(coder.jump(jump))
            except KeyError:
                f.write(jump)
            f.write("\n")


f.close()
parser2.file.close()
