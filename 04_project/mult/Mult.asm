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

//R0의 값을 R1번 더하는 방법으로 곱셈하기
    @2
    M=0 //R[2] = 0
    @i
    M=1 //i = 1
(LOOP)
    @i
    D=M //D = i
    @1
    D=D-M //D = i - R[1], 이 값이 음수일동안 반복
    @END
    D;JGT

    @0
    D=M //D = R[0]
    @2
    M=D+M //R[2] = R[0] + R[2]
    @i
    M=M+1 //i = i + 1
    @LOOP
    0;JMP //Goto Loop
(END)
    @END
    0;JMP //무한루프
