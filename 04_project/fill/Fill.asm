// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
//키를 누르고 있는 동안은 스크린이 검은색으로 채워지고, 아무 키도 누르고 있지 않으면 검은색 셀이 하나씩 지워짐
//i=0부터 하나씩 증가시키면서 눌리면->-1 and i++, 안눌리면-> 0 and i--
    @i
    M=0; //i = 0
(LOOP)
    @KBD
    D=M
    @PRESSED
    D;JNE //키보드 값이 0이 아닌 경우 PRESSED로 이동
    //NOT PRESSED, i값>=0인 동안 스크린 픽셀 흰색으로 변화시킴
    @NOTPRESSED
    D;JEQ

(PRESSED)
    //i값 증가 + 스크린 픽셀 검정색으로 변화시킴
    @i
    D=M // D = i
    @8191
    D=D-A
    @LOOP
    D; JGT //끝까지 찰 때까지 계속함
    @i
    D=M // D = i
    @SCREEN
    A=A+D //D = SCREEN 시작 주소값 + i
    M=-1 //M[A] = -1
    @i
    M=M+1 //i = i + 1
    @LOOP
    0; JMP

(NOTPRESSED)
    @i
    D=M // D = i
    @PAUSE
    D;JLT // 만약 SCREEN범위를 벗어날 경우, 일시 정지
    @i
    D=M // D = i
    @SCREEN
    A=A+D // D = SCREEN 시작 주소값 + i
    M=0 // 입력되지 않을 경우니까 0으로
    @i
    M=M-1 // i = i - 1
    @LOOP
    0;JMP

(PAUSE)
    @i
    M=0
    @LOOP
    0;JMP