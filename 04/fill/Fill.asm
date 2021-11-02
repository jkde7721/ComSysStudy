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

@SCREEN
D=A
@IDX
M=D
(LOOP)
  @24576
  D=M
  @ELSE
  D;JEQ // 키보드 값 0이면 ELSE로
  @IDX
  D=M
  A=M
  M=-1
  D=D+1
  @IDX // IDX에 다음 WORD위치
  M=D
  @LOOP
  0;JMP

  (ELSE)
    @IDX
    D=M
    A=M
    M=0
    D=D-1
    @IDX // IDX에 바로 앞 WORD위치
    M=D
    @LOOP
    0;JMP
