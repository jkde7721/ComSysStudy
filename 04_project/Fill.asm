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
(LOOP)
@KBD
D=M

@TOBLACK
D;JGT // a key is pressed
@TOWHITE
D;JEQ // no key is pressed

(TOBLACK)
@0
M=-1 // 1111 1111 1111 1111

@FILL
0;JMP // goto FILL

(TOWHITE)
@0
M=0

@FILL
0;JMP // goto FILL

(FILL)
@0
D=M // D= -1 or 0

@SCREEN
M=D

// 첫 16비트는 칠했는데 그 이후는 잘 모르겠음
// 인덱스의 값을 키우는 방법을 모르겠음

@LOOP
0;JMP // goto LOOP
