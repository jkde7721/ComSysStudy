# Ch03 순차 논리

컴퓨터는 **값을 계산**(by. 불 논리 칩, 산술 칩)할 뿐만 아니라 필요할 때마다 **저장하고 다시 불러올 수 있어야** 함, 즉 데이터를 보존할 수 있는 메모리 소자가 필요 ⇒ by. 순차 칩(sequential chip)

메모리 소자 구현은 동기화(synchronization), 클로킹(clocking), 피드백 루프(feedback loop) 등을 포함한 복잡한 기술 

⇒ **플립플롭**(flip-flop)이라는 매우 저수준의 순차 게이트가 이 복잡한 논리 모두 수행, 기본 구성 블록 

### 클록

- 마스터 클록(master clock)
    - 연속적으로 신호를 발생
    - 시간 진행을 표현
    - 0-1 또는 틱-톡 등으로 표시된 두 신호 상태를 연속해서 오가는 발진기로 구현
- 사이클(cycle)
    - '틱'의 시작에서 다음번 '톡'의 끝까지 경과된 시간
    - 하나의 시간 단위 구분을 모델링
    - 2진 신호로 표현 ⇒ 신호는 하드웨어의 회로망을 통해 컴퓨터에 있는 모든 순차 칩들에 동시 전달

### 플립플롭

- **DFF**(Data Flip-Flop) : 1비트 데이터 입력과 1비트 데이터 출력으로 구성된 인터페이스 + 클록 입력 신호
    
    ⇒ 데이터 입력과 클록 입력을 종합해서 시간에 따른 동작 **out(t) = in(t-1)** 수행, 즉 전 시간의 입력을 출력 (in : 입력값, out : 출력값, t : 현재 클록 사이클) 
    
- 모든 메모리 소자의 기본 부품, 1비트 입력 → 1비트 출력
- 하나의 마스터 클록에 연결
- 클록 사이클 외의 시간에는 DFF 잠금(latch) 상태  ⇒ 입력이 변해도 출력이 곧바로 영향을 받지 않음
- DFF에 기반한 모든 상위 계층 칩들 또한 시간의존성O

### 레지스터

- 시간이 지나도 값을 '**저장**' '**로드**'할 수 있는 장치, 기본 저장 기능 **out(t) = out(t-1)** 구현
    
    ⇒ 멀티플렉서 도입 
    
- '선택 비트' = 레지스터의 '로드 비트(load bit)'
- 입력 in에 새로운 값을 넣고 **로드 비트를 1**로 설정 ⇒ 새로운 값 저장
- **로드 비트 0**으로 설정 ⇒ 다음번 변경 때까지 내부 값을 계속 유지
- 멀티비트 값을 저장하는 레지스터 = 1비트 레지스터를 필요한 만큼 배열
    
    폭(width) : 레지스터가 저장할 수 있는 비트의 개수
    
    단어(word) : 레지스터에 저장되는 멀티비트 개수 
    
- 2진 셀(binary cell) : 하나의 정보 비트를 저장하도록 설계된 소자 (1비트 레지스터)
- 데이터 비트를 전달하는 **입력 핀**, 쓰기 기능을 설정하는 **로드 핀**, 셀의 현대 상태를 내보내는 **출력 핀**

### 메모리

- RAM(Random Access Memory) : **레지스터를 여러 개** 쌓아 올려서 만듦, 직접 접근(direct-access) 메모리 장치, n개의 w-비트 레지스터를 배열하고 직접 접근 회로를 연결한 소자
- 임의 접근 메모리
    - 접근 순서와 관계없이 무작위로 선택된 단어를 읽고 쓸 수 있음
    - 물리적 저장 위치에 관계없이 똑같은 속도로 직접 접근 가능

⇒ **데이터 입력**, **주소 입력**(현재 시간 사이클에서 어떤 RAM 레지스터에 접근할지 가리킴), **로드 비트**의 3가지 입력

- RAM 장치의 기본 변수
    
    데이터 폭(width) : 각 단어(레지스터)의 비트 수(w) 
    
    크기(size) : RAM 내의 단어 수, 메모리에 들어간 레지스터의 개수(n)
    

### 계수기

- 매 시간 단위마다 내부 상태 값을 증가시키는 순차 칩, 로드/리셋/증가 기능
- **out(t) = out(t-1) + c** (c는 보통 1)

⇒ 표준 레지스터의 입력/출력 논리 + 상태 값에 상수를 더하는 논리의 조합 

- 계수기의 제어 비트
    
    **reset** = 1 ⇒ 계수기를 0으로 재설정 
    
    in = d, **load** = 1 ⇒ 계수기를 d로 초기화  
    
    **inc** = 1 ⇒ 매 클록 사이클마다 상태 값을 1 증가, out(t) = out(t - 1) + 1
    

### 순차칩

- 직접적이든 간접적이든 **하나 이상의 DFF 게이트를 장착**한 칩
- 상태를 유지 or 상태를 조작
- 순차 칩 내부에 피드백 루프(feedback loop, 출력 신호를 다시 입력으로 넣는 구조)를 넣는 방식으로 구현
- DFF에 시간 지연O ⇒ 시간 t의 출력은 자기 자신이 아니라 시간 t-1의 출력을 따름
- 순차 칩의 출력은 한 클록 사이클이 넘어갈 때 변경, 즉 한 사이클 안에서는 바뀌지 않음, 다음 사이클을 시작하는 순간에만 올바른 값을 출력하면 됨
- 이산화
    - 연속적인 신호를 잘게 쪼개서 서로 구분되는 **불연속적인 신호**로 바꾸는 것
    - 순차 칩의 출력이 연속적으로 변하지 않고 **클록 사이클 단위로 변화**하는 것

### 조합 칩

- 시간 인지X ⇒ 출력→입력 / 입력→출력, 출력이 자기 자신을 따라 결정된다는 문제!, 즉 데이터 경쟁(data race) 발생!
- 조합 칩의 출력은 시간과 무관, 입력이 바뀔 때만 변경
