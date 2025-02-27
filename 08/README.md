# Ch08 가상 머신 II: 프로그램 제어

### 고수준 언어의 세 가지 특성

1. 고급 연산을 필요에 따라 자유롭게 정의 가능 
2. 서브루틴을 기본 연산처럼 자유롭게 사용(호출) 가능 
3. 호출된 서브루틴이 실행되고 나면 그 종료 제어가 코드 내의 다음 명령으로 반환 

<br>

### 서브루틴 호출 시, 필요한 처리

1. 호출자에서 호출된 서브루틴으로 **매개변수 전달** 
2. 호출된 서브루틴을 시행하기 전에 **호출자 상태 저장** 
3. 호출된 서브루틴의 **지역 변수에 공간 할당** 
4. 호출된 서브루틴을 실행하기 위해 **점프**
5. 호출된 서브루틴의 **결과값을 호출자로 반환** 
6. 값이 반환될 때, 호출된 서브루틴이 점유한 메모리 공간 재활용 
7. **호출자 상태 복구**
8. 호출자 코드에서 이전에 점프했던 지점 바로 다음 코드를 실행하기 위해 **점프** 

<br>

## 8.1.1 프로그램 흐름 제어

컴퓨터 프로그램은 한 명령어씩 순차적으로 실행하는 방식이 기본 

- **goto destination** : 프로그램 내 특정 위치부터 실행 지속
    
    *위치 설정 방식 : 다음에 실행될 명령어의 물리적 주소 지정하는 방식 
    
     (코드 내 특정 지점을 뜻하는 **기호 레이블**을 통해 점프할 위치 지정)
    
- **if-goto destination** : 불 조건문이 참일 경우만 점프, 거짓이면 원래 프로그램 흐름대로 바로 다음번 명령 실행

<br>

### 저수준 제어 흐름

```basic
// 제어 흐름 구조
if (cond)
  s1
else 
  s2 

// 의사 VM 코드 
  VM code for computing ~(cond)
  if-goto L1
  VM code for executing s2
  goto L2
label L1
  VM code for executing s1
label L2 
```

```basic
// 제어 흐름 구조 
while (cond)
  s1
  ...

// 의사 VM 코드 
label L1
  VM code for computing ~(cond)
  if-goto L2
  goto L3
label L2
  VM code for executing s1
  goto L1 
label L3 
  ...
```

<br>

## 8.1.2 서브루틴 호출

- 고수준 연산 단위 = 서브루틴, 프로시저, 함수, 메서드
- 기초 명령(내장형 명령, add)과 고수준 연산(사용자 정의 서브루틴, power)은 같은 방식으로 **인수 처리**하고 **값 반환**
    
    (차이점 : 서브루틴 앞에 **call** 키워드 쓰임)
    
- 호출 및 반환 논리의 단계적인 특성 : 서브루틴 호출이 아무리 많이 중첩되어 있더라도, 어느 한 시점에는 중첩된 서브루틴들 중 맨 위의 것만 실행, 그 아래 다른 서브루틴 호출들은 현재 실행되는 서브루틴이 종료되기를 기다림

- **프레임**(frame) : 서브루틴의 **지역 변수**, 서브루틴이 연산할 **인수**들, **작업 스택**과 연산을 지원하는 **메모리 세그먼트** 통칭
- **전역 스택**(global stack) : 현재 서브루틴과, 그 서브루틴의 반환을 대기하는 다른 모든 서브루틴들의 **프레임**을 담고 있는 메모리 영역 (현재 서브루틴의 작업 스택이 전역 스택의 맨 위에 위치)

<br>

### 저수준에서 call xxx 연산 구현

1. 호출자의 프레임을 스택에 저장 
2. 호출된 서브루틴의 지역 변수에게 스택 공간 할당 
3. 실행할 코드로 점프 
    
    (**call** 명령에 명시된 서브루틴의 메모리 주소를 확인하고 그 주소로 시작하는 실행 코드로 점프)
    
- 호출된 서브루틴에서 **return** 명령으로 반환할 때 → 반환될 주소 명령에 표시X
    
    ⇒ 현재 서브루틴을 호출한 call 명령이 프로그램 어디에 있든지 간에 그 다음 명령으로 점프해서 실행 (**다음 명령의 메모리 위치** = **반환 주소**)
    
<br>

## 8.2.1 프로그램 흐름 제어 명령 명세

- **label label** : 현재 함수 코드 위치에 레이블 붙임 (레이블이 정의된 함수 내부에서만 레이블 유효)
- **goto label** : 무조건 분기 명령, label 위치부터 실행 (점프할 위치는 같은 함수 내)
- **if-goto label** : 조건 분기 명령, 스택의 최상단 값을 꺼내서 그 값이 0이 아니면 label로 표시된 위치에서 계속 실행, 값이 0이면 프로그램 다음 명령 실행 (점프할 위치는 같은 함수 내)

<br>

## 8.2.2 함수 호출 명령 명세

함수 이름의 번위는 전역적 (모든 파일 내의 함수 이름들이 공유, 함수 이름으로 서로 호출)

- **function f n** : 이름이 f, 지역 변수가 n개인 함수 코드 시작
- **call f m** : 함수 f 호출 (호출자가 스택에 m개의 인수 푸시)
- **return** : 호출한 함수로 반환

<br>

## 8.2.3 함수 호출 규약

VM 함수의 실행이 시작될 때 **메모리 세그먼트**들과 **스택**들로 이루어진 개별 환경으로 둘러싸임

⇒ VM이 VM 함수마다 이러한 가상 환경을 만들어줌 

### 호출하는 함수 관점

- 함수 호출 전 필요한 수의 **인수 스택에 푸시**
- **call** 명령으로 **함수 호출**
- 호출된 함수가 반환된 후, 호출하기 전 푸시한 인수들은 스택에서 사라지고 **반환값은 스택 최상단에 위치**
- 호출된 함수가 반환된 후, 호출자의 메모리 세그먼트들(argument, local, static, this, that, pointer)은 **호출 전과 동일**, temp 세그먼트는 미정의 상태

### 호출되는 함수 관점

- 호출된 함수가 실행을 시작할 때
    - argument 세그먼트 : 호출자가 넘겨준 실제 인수값으로 초기화
    - local 세그먼트 : 할당 후 0으로 초기화
    - static 세그먼트 : 그 함수가 속하는 VM 파일의 static 세그먼트로 설정
    - this, that, pointer, temp 세그먼트 : 시작 전에는 미정의 상태
    - 작업 스택은 empty
- 반환되기 전 **값 하나 스택에 푸시**

<br>

## 8.2.4 초기화

- VM 프로그램 = VM 함수들의 모음
- VM이 실행될 때(또는 리셋될 때), 항상 **Sys.init** 이라는 인수 없는 VM 함수 먼저 실행!!
- **Sys.init** : 사용자 프로그램의 **메인 함수 호출**

<br>

## 8.3.1 핵 플랫폼에서 표준 VM 매핑, 2부

### 전역 스택

- 함수가 호출될 때마다 전역 스택에는 새로운 블록 추가
- 블록은 호출된 함수에서 사용하는 **인수**, **지역변수**, **빈 작업 스택**과 호출하는 함수의 상태를 저장하는 **포인터**로 구성

### 함수 호출 규약 구현

**call f n** : n개의 인수를 스택에 푸시한 후 함수 f 호출 

```basic
push return-address // 아래 선언된 레이블 사용 
push LCL // 호출한 함수의 LCL 저장 
push ARG // 호출한 함수의 ARG 저장 
push THIS // 호출한 함수의 THIS 저장 
push THAT // 호출한 함수의 THAT 저장 
ARG = SP-n-5 // ARG 위치 재설정 (n은 인수 개수, 5는 호출한 함수의 프레임 개수)
LCL = SP // LCL 위치 재설정 
goto f // 제어 이동 
(return-address) // return-address 레이블 선언 
```

**function f k** : k개의 지역 변수가 있는 함수 f 선언 

```basic
(f) // 함수 시작 레이블 선언 
repeat k times: // k는 지역 변수 개수 
PUSH 0 // 지역 변수 모두 0으로 초기화 
```

**return** : 어떤 함수에서 반환 

```basic
FRAME = LCL // FRANE은 임시 변수 
RET = *(FRAME-5) // return-address 임시 변수에 저장 
*ARG = pop() // 호출자 위해 반환값 위치 재설정(함수의 결과값 첫번째 인수 자리에 저장)
SP = ARG + 1 // 호출자의 SP 복구 
THAT = *(FRAME-1) // 호출자의 THAT 복구 
THIS = *(FRAME-2) // 호출자의 THIS 복구 
ARG = *(FRAME-3) // 호출자의 ARG 복구 
LCL = *(FRAME-4) // 호출자의 LCL 복구 
goto RET // (호출자 코드 내) return-address로 점프
```

<br>

### 어셈블리 언어 기호들

|**기호**|**사용법**| 
|--------|----------|
|**functionName$label**|VM 함수 f 내의 label b 명령마다 전역적으로 유일한 기호 **f$b** 생성 (goto b, if-goto b 명령 번역 시, b 대신 전체 레이블 f$b 사용)|
|**(FunctionName)**|VM 함수 f마다 명령어 메모리 내 시작 포인트를 가리키는 기호 **f** 생성|
|**return-address**|VM 함수마다 **반환 주소**, 즉 함수가 호출된 곳 바로 다음 명령의 메모리 주소를 가리키는 유일한 기호 생성|  

<br>

### 부트스트랩 코드 : 컴퓨터가 부팅될 때 처음 실행되는 코드

- 번역된 .asm 파일이 준수해야 하는 규칙
    - VM 스택은 RAM[**256**] 위치부터 매핑
    - 실행되는 첫 번째 VM 함수는 **Sys.init**

### 핵 컴퓨터의 부트스트랩 코드

```basic
SP=256 // 스택 포인터를 0x0100으로 초기화 

call Sys.init // 번역된 Sys.init 실행 시작
```

**Sys.init** : 메인 프로그램의 메인 함수 호출 → 무한 루프 → 번역된 VM 프로그램 실행
