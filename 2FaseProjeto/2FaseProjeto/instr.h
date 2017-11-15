#ifndef INSTR_H
#define INSTR_H
#include "util.h"
#include "arena.h"
/* Códigos das instruções */

typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STS,
  RCS,
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  STL,
  RCE,
  ALC,
  FRE,
  ATR,
  MOV,
  ATK,
  DEP,
  INF,
  CLT
} OpCode;

/* Tipos dos operandos */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELL,
  BOOL,
  NONE
} Tipo;
/* enum de Direção. Usada para converter o código dado pelo usuário
  em um número, no intervalo [0, 5], sendo associado a cada palavra na ordem
   disposta na ENUM. Então:
   0 - norte, 1 - nordeste, 2 - sudeste, 3 - sul, 4 - sudoeste, 5 - noroeste */
typedef enum {
  NORTH,
  NORTHEAST,
  SOUTHEAST,
  SOUTH,
  SOUTHWEST,
  NORTHWEST,
  CURRENT
}Dir;
/* Operando, agora com o atributo Tipo */
/***************************************
* t = tipo da variavel
* n = valor da variavel de tipo NUM
* ac = valor da variavel do tipo ACAO
* v = variavel do tipo VAR
* c = célula do tipo CELL
* b = valor booleano BOOL
* d = Direção
***************************************/
typedef struct {
   Tipo t;
  union Valor {
 	  int n;
 	  int ac;
 	  int v;
    Celula c;
    Boolean b;
    Dir d;
  } Valor;
 } OPERANDO;
/* Instrução
Toda instrução possui um Opcode, um Tipo de variável e um conjunto de valores
Se a função não tem argumentos, o tipo é definido para NONE e os valores para 0*/
typedef struct {
  OpCode instr;
  OPERANDO o;
} INSTR;
#endif /* INSTR_H*/
