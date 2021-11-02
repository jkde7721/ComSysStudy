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
@i
M=0 // i=0

@KBD
D=M

@TOBLACK
D;JGT // a key is pressed. If D>0 goto TOBLACK
@TOWHITE
D;JEQ // no key is pressed. If D=0 goto TOWHITE

(TOBLACK)
@SCREEN
D=A // D=16384

@i
M=M+16 // 16 픽셀씩 칠해지므로
A=M // A=i

A=A+D // A=16384+i
M=-1 // M[A]=-1(1111 1111 1111 1111)

D=A
@KBD
D=D-A // D=(16384+i)-24576
@TOBLACK
D;JLT // If D<0 goto TOBLACK. 다 칠해지도록

@LOOP
0;JMP // goto LOOP

(TOWHITE)
@SCREEN
D=A // D=16384

@i
M=M+16 // 16 픽셀씩 칠해지므로
A=M // A=i

A=A+D // A=16384+i
M=0 // M[A]=0

D=A
@KBD
D=D-A // D=(16384+i)-24576
@TOWHITE
D;JLT // If D<0 goto TOWHITE. 다 칠해지도록

@LOOP
0;JMP // goto LOOP
