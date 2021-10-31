// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.
// 초기화
@2
M=0 // Memory[2]=0

// Memory[0]=0일 경우
@0
D=M // D=Memory[0]
@END
D;JEQ // If D=0 goto END

// Memory[1]=0일 경우
@1
D=M // D=Memory[1]
@END
D;JEQ // If D=0 goto END

(LOOP)
@0
D=M // D=Memory[0]

@2
M=M+D // Memory[2]=Memory[2]+D

@1
M=M-1 // Memory[1]=Memory[1]-1

@LOOP
M;JGT // If Memory[1]>0 goto LOOP

(END)
@END
0;JMP	// 무한 루프
