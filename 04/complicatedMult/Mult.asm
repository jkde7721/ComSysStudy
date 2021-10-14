// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.
// 복잡한 곱셈 연산 
// 2 * 10의 연산 
// 2 = 00000010, 10 = 00001010
// 2 * 8 = (00000010) << 3 + (00000010) << 1 = 00010000 + 00000100 = 16 + 4 = 20 

    @sum
    M=0
    @idx 
    M=0 
    @digit
    M=1 

(LOOP)
    @16
    D=A
    @idx
    D=D-M
    M=M+1 // 다음 인덱스 미리 지정 
    @STORE
    D; JLE // idx >= 16 이면, 결과 R2에 저장 -> END 
    @digit // 각 비트가 1일 때의 값, ex. 00000010의 경우 digit은 2 
    D=M // D+D 또는 M+M 연산은 불가능 
    M=M+D // 비트 shift 연산이 없어 덧셈으로 2의 거듭제곱 구현, 다음 상위 비트를 위한 값 미리 지정 
    @R1
    D=D&M
    @LOOP
    D; JEQ // 해당 자리 비트는 0 -> 곱셈 연산의 대상X    
    @idx
    D=M
    @times // shift 연산 수행 횟수
    M=D-1 // 위에서 증가시킨 값 때문 
    @R0
    D=M
    @tmp
    M=D
(SHIFT)  
    @times
    D=M
    @SUBSUM
    D; JLE // times <= 0 이면, shift 연산 끝 -> 앞의 결과에 더함 
    @tmp
    D=M 
    M=M+D // shift 연산 수행에 따른 (현재 값*2) 연산 수행 
    @times
    M=M-1
    @SHIFT
    0; JMP     
    
(SUBSUM)
    @tmp
    D=M 
    @sum
    M=M+D
    @LOOP
    0; JMP

(STORE)
    @sum
    D=M
    @R2
    M=D
    @END
    0; JMP 

(END)
    @END
    0; JMP  