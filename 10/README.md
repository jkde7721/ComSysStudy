# Ch10 컴파일러 I: 구문 분석

## 컴파일러

- **구문 분석**(syntax analysis) → **코드 생성**(code generation)
- **구문 분석** : 프로그램의 구조를 해석하는 역할
    
    ⇒ 프로그램의 **텍스트**와 **문법 규칙** 사이에 정확한 대응 관계를 결정
    
    - **토큰화**(tokenizing) : 입력 문자들을 언어 기본 요소들로 분류하는 모듈
    - **파싱**(parsing) : 토큰화 결과로 나온 언어 기본 요소 스트림을 언어의 구문 규칙에 맞추는 모듈
        
        ⇒ 소스 프로그램을 번역할 대상 언어와 완전히 독립적 
        
<br>

## 10.1.1 어휘 분석

- 공백이나 주석은 무시하고 문자들을 토큰들로 분류
- 토큰 : 프로그램의 기본 원소
- 토큰 스트림 → 컴파일러의 입력

```c
while (count <= 100) { /** some loop */
  count++;
  // Body ofwhile continues 
  ...
}
```
=> `while` `(` `count` `<=` `100` `)` `{` `count` `++` `;` `}` 

<br>

## 10.1.2 문법

### 파싱(parsing)

- 토큰 스트림을 분석해서 형식 구조로 만드는 일
- 토큰들이 변수 선언, 명령문, 표현식 등과 같은 언어 구조 중에 어디에 해당하는지 알아냄
- 토큰 스트림을 문법(미리 정의된 규칙들)에 맞춰보는 작업

**문맥 자유 문법**(context-free grammar) : 어떤 언어의 구문 요소들을 더 단순한 요소들을 이용해 구성하는 규칙 (**문법 규칙은 재귀적!**)

<br>

## 10.1.3 구문 분석

### 텍스트 분석

- 텍스트와 문법 규칙 사이의 정확한 대응 관계를 결정하는 일
- **문법 규칙 계층적** → 분석기가 생성하는 출력은 **파스 트리** 혹은 **유도 트리**

### 재귀적 하향 구문 분석

문법에 정의된 중첩 구조를 이용해 토큰 스트림을 재귀적으로 분석 

**How** : 비단말 문법의 모든 규칙마다, 그 규칙을 분석하기 위한 재귀적 루틴들을 하나씩 만들어 넣음 

→ 단말 원소 : only 단순한 처리 

→ 비단말 원소 : 해당 비단말들을 분석하는 루틴을 **재귀적으로 호출** 

⇒ 단말 원소들만 처리될 때까지 재귀적으로 반복 

### LL(0) 문법

- 어떤 비단말에서 유도되는 규칙이 여러 개인 경우, 그중 어떤 규칙을 적용할지는 항상 **첫 번째 토큰만 보고도 결정**할 수 있다.
- but. 첫 번째 토큰만으로 요소의 종류가 결정되지 않는다면 그 다음 토큰을 **‘미리 살펴봄’**으로써 해결 가능 → 잭언어에서는 표현식(expression) 분석 시, 다음 토큰을 미리 살펴봐야 함

<br>

## 10.2.1 잭 언어 문법

### 단말 요소(토큰)

- keyword : `class` `constructor` `function` `method` `field` `static` `var` `int` `char` `boolean` `void` `true` `false` `null` `this` `let` `do` `if` `else` `while` `return`
- symbol : `{` `}` `(` `)` `[` `]` `.` `,` `;` `+` `-` `*` `/` `&` `|` `<` `>` `=` `~`
- integer Constant(정수 상수) : 0-32767의 10진 **숫자**
- String Constant(문자열 상수) : 따옴표와 줄바꿈 문자를 제외한 유니코드 **문자열**
- identifier(식별자) : 숫자로 시작하지 않는 **영문자, 숫자, `_`로 이뤄진 문자열** (변수?)

### 비단말

- **프로그램 구조**
    - class : `class` **className** `{` **classVarDec*** **subroutineDec*** `}`
    - classVarDec : (`static` | `field`) **type** **varName** (`,` **varName**)* `;`
    - type : `int` | `char` | `boolean` | **className**
    - subroutineDec : (`constructor` | `function` | `method`) (`void` | **type**) **subroutineName** `(` **parameterList** `)` **subroutineBody**
    - parameterList : ((**type** **varName**) (`,` **type** **varName**)*)?
    - subroutineBody : `{` **varDec*** **statements** `}`
    - varDec : `var` **type** **varName** (`,` **varName**)* `;`
    - className : **identifier**
    - subroutineName : **identifier**
    - varName : **identifier**

- **명령문**
    - statements : **statement***
    - statement : **letStatement** | **ifStatement** | **whileStatement** | **doStatement** | **returnStatement**
    - letStatement : `let` **varName** (`[` **expression** `]`)? `=` **expression** `;`
    - ifStatement : `if` `(` **expression** `)` `{` **statements** `}` (`else` `{` **statements** `}`)?
    - whileStatement : `while` `(` **expression** `)` `{` **statements** `}`
    - doStatement : `do` **subroutineCall** `;`
    - returnStatement : `return` **expression**? `;`

- **표현식**
    - expression : **term** (**op** **term**)*
    - term : **integerConstant** | **stringConstant** | **keywordConstant** | **varName** | **varName** `[` **expression** `]` | **subroutineCall** | `(` **expression** `)` | **unaryOp** **term**
    - subroutineCall : **subroutineName** `(` **expressionList** `)` | (**className** | **varName**) `.` **subroutineName** `(` **expressionList** `)`
    - expressionList : (**expression** (`,` **expression**)*)?
    - op : `+` | `-` | `*` | `/` | `&` | `|` | `<` | `>` | `=`
    - unaryOp : `-` | `~`
    - keywordConstant : `true` | `false` | `null` | `this`

<br>

## 10.2.2 잭 언어 구문 분석기

잭 프로그램을 읽어서 잭 문법에 따라 그 구문 구조를 이해하는 것 

<br>

## 10.2.3. 구문 분석기 입력

Xxx.jack 형식의 파일명 혹은 하나 이상의 .jack 파일들을 담고 있는 디렉터리명 → Xxx.xml 파일 생성 

<br>

## 10.2.4 구문 분석기 출력

### **단말**(terminal)

- 구문 분석기가 타입이 xxx인 단말 언어 요소 만나면
    
    `<xxx> terminal </xxx>` 출력 생성 
    
- **xxx** : keyword, symbol, integerConstant, stringConstant, identifier

### **비단말**(non-terminal)

- 구문 분석기가 타입이 xxx인 비단말 언어 요소 만나면
    
    `<xxx> xxx요소의 본문에 대한 재귀 코드 </xxx>` 출력 생성 
    
- **xxx** : class, classVarDec, subroutineDec, parameterList, subroutineBody, varDec / statements, whileStatement, ifStatement, returnStatement, letStatement, doStatement / expression, term, expressionList

⇒ 파스 트리에서 **단말 노드**(토큰)**들**이 묶이는 상위 구조가 **비단말 노드**
