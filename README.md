# Compiler for ALGUMA

## 1. Introduction
This is a college project. A compiler fot the made up language 'ALGUMA' (**ALG**oritmos **U**sados para **M**ero **A**prendizado - *Algorithms used solely for learning purposes*)

ALGUMA is a very simple language, consisting of the following:

Type | Symbols
-----|--------
Keywords | DECLARACOES, ALGORITMO, INTEIRO, REAL, ATRIBUIR, A, LER, IMPRIMIR, SE, ENTAO, ENQUANTO, INICIO, FIM
Aritmetic Operators | *, /, +, -
Relational Operators | <, <=, =, >=, >, <>
Boolean Operators | E, OU
Delimiter | :
Parentheses | (, )
Variables | `[a-z][a-zA-Z0-9]*`
Integers | `[0-9]+`
Floats | `[0-9]+\.[0-9]+`
Strings | `".*"`

## 2. Compiling
Just run the Makefile inside a GNU/Linux shell (I haven't tried to run the Makefile on a windows shell but it will probably not run) with the `g++`, `g++-mingw-w64-x86-64` and `make` packages. The Makefile has 2 main rules: **dev** and **dist**, you can run them by jsut typing:
```
make dev
```
or
```
make dist
```
The differences are:
* **dev** will compile **only the linux version** and place the executable at `./compiler.exe`
* **dist** will compile both **linux and win64 versions** and place the executables at `dst/compiler_[linux|win64].exe`

## 3. Usage
**This will be changed over the course of the development, that is not yet finished**. What is already done:
* ~~Lexical Analyzer~~
* ~~Syntactic Analyzer~~
* Semantic Analyzer
* Intermediary code generator (C)

The compiler only compiles one file and there's no importing. At the current state you can run it as:
```
compiler.exe [flags] [file]
```
>The order between flags and file doesn't matter.

The current flags are:
Flag | Meaning | Values expected
-|-|-
-h | Show help message | 
-l | Set log level | info, success, warn, error, severe

The compiler doesn't yet output any code, it only says if the lexical and syntactic testes succeed, and, if not, where is the error.

## 4. Examples

### 4.1. OK
```
:DECLARACOES
argumento:INT
fatorial:INT

:ALGORITMO
% Calcula o fatorial de um número inteiro
LER argumento
ATRIBUIR argumento A fatorial
SE argumento = 0 ENTAO ATRIBUIR 1 A fatorial
ENQUANTO argumento > 1
   INICIO
      ATRIBUIR fatorial * (argumento - 1) A fatorial
      ATRIBUIR argumento - 1 A argumento
   FIM
IMPRIMIR fatorial
```
```
[SUCCESS] OK 0.79ms
```

### 4.2. Lexical Error
```
:DECLARACOES
argumento:INT
fatorial:INT

:ALGORITO
% Calcula o fatorial de um número inteiro
LER argumento
ATRIBUIR argumento A fatorial
SE argumento = 0 ENTAO ATRIBUIR 1 A fatorial
ENQUANTO argumento > 1
   INICIO
      ATRIBUIR fatorial * (argumento - 1) A fatorial
      ATRIBUIR argumento - 1 A argumento
   FIM
IMPRIMIR fatorial

```
```
[ERROR] Lexical error!: Near 'A', On position 42, Line 5
```

### 4.3. Syntactic Error
```
:DECLARACOES
argumento:INT
fatorial:INT

:ALGORITMO
% Calcula o fatorial de um número inteiro
LER argumento
ATRIBUIR argumento A 1
SE argumento = 0 ENTAO ATRIBUIR 1 A fatorial
ENQUANTO argumento > 1
   INICIO
      ATRIBUIR fatorial * (argumento - 1) A fatorial
      ATRIBUIR argumento - 1 A argumento
   FIM
IMPRIMIR fatorial

```
```
[ERROR] Syntax error!: expecting one of the following (lng_Var), but found 'lng_Int', On position 130, Line 8
```