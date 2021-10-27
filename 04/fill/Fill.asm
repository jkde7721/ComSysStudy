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
    D = A 
    @cur
    M = D // 사용자 정의 변수 초기값 cur = SCREEN, RAM16에 저장, 유효하지 않은 값을 가진 스크린 픽셀의 주소 have

(LOOP) 
    @KBD
    D = M // 키보드로 입력된 값 
    @NOTPUSH
    D; JEQ // memory[KBD] == 0 이면 점프 
    @24576 // 스크린 마지막 픽셀의 주소 + 1(=키보드 시작 주소), 상수 자체 
    D = A 
    @cur
    D = M - D
    @LOOP
    D; JGE // cur >= (end + 1) 이면 더이상 검은색 표시X, 스크린 픽셀의 범위 넘어간 경우
    @cur
    A = M // 해당 스크린의 주소 
    M = 1 // 해당 스크린 픽셀 1로 설정, 검은색 
    @cur 
    M = M + 1; // 스크린의 다음 픽셀 주소 
    @LOOP
    0; JMP

(NOTPUSH)
    @cur
    D = M
    @SCREEN
    D = D - A
    @LOOP
    D; JLE // cur <= SCREEN 이면 while문 앞으로  
    @cur
    M = M - 1
    A = M
    M = 0 // 흰색 
    @LOOP
    0; JMP 
