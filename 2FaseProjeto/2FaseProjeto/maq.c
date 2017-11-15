#include <stdio.h>
#include <stdlib.h>
#include "maq.h"
/* #define DEBUG */

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "STS",
  "RCS",
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "STL",
  "RCE",
  "ALC",
  "FRE",
  "MOV", // Opcodes adicionados daqui para baixo
  "ATK",
  "INF",
  "CLT",
  "DEP",
  "ATR"
};
#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}
/*
Maquina *cria_maquina(INSTR *p)
{

}
*/
Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->prog = p;
  m->posx = posX;
  m->posy = posY;
  m->vida = vidaV;
  m->crist = CrstV;
  m->time = TimeV;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)

void exec_maquina(Maquina *m, int nInstrucoes) {
  // variáveis de iteração
  int i;
  int indexALC = 0;
  // inteiro auxiliar, para iterações e atribuições
  int temp = 0;
  // Operandos auxiliares, para efetuar operações com mais de um operando
  OPERANDO a,b;
  for (i = 0; i < nInstrucoes; i++) {
    // recuperar OpCode da instrução
     OpCode opc = prg[ip].instr;
     //construir OPERANDO (arg) a partir da instrução
	    OPERANDO arg = prg[ip].o;

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));
  /*
  MOV, ATK, INF estão prontos. Olhar funcionamento.
  */
	switch (opc) {
	  OPERANDO tmp;
	case PUSH:
    //Empilhar um objeto
	  empilha(pil, arg);
	  break;
	case POP:
    // Desempilhar um objeto
	  desempilha(pil);
	  break;
	case DUP:
    // Duplicar o objeto no topo da pilha
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD:
    // Desempilhar e somar os dois objetos no topo da pilha e
    // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
    // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
    // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      empilha(pil, (OPERANDO){NUM,a.Valor.n + b.Valor.n});
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: ADD só definido para o tipo NUM");
    }
	  break;
	case SUB:
    // Desempilhar e subtrair os dois objetos no topo da pilha e
    // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
    // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
    // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      empilha(pil, (OPERANDO){NUM, b.Valor.n - a.Valor.n});
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: SUB só definido para o tipo NUM");
    }
    break;
	case MUL:
  // Desempilhar e multiplicar os dois objetos no topo da pilha e
  // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
  // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
  // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      empilha(pil, (OPERANDO){NUM,a.Valor.n * b.Valor.n});
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: MUL só definido para o tipo NUM");
    }
    break;
	case DIV:
  // Desempilhar e dividir os dois objetos no topo da pilha e
  // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
  // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
  // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      if (b.Valor.v == 0)
      {
        empilha(pil, b);
        empilha(pil, a);
        Erro("Erro: Divisão por zero.");
      }
      empilha(pil, (OPERANDO){NUM,a.Valor.n / b.Valor.n});
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: DIV só definido para o tipo NUM");
    }
    break;
	case JMP:
    // Colocar o ip na instrução dada no argumento
	  ip = arg.Valor.n;
	  continue;
	case JIT:
    // Se o valor no topo da pilha for verdadeiro,
    // posicionar o ip na posição dada pelo argumento
	  if (desempilha(pil).Valor.b != false) {
	    ip = arg.Valor.n;
	    continue;
	  }
	  break;
	case JIF:
  // Se o valor no topo da pilha for false,
  // atribuir ao ip na posição dada pelo argumento
	  if (desempilha(pil).Valor.n == false) {
  		ip = arg.Valor.n;
  		continue;
	  }
	  break;
	case CALL:
    // empilhar o ip atual e atribuir o valor do argumento da função ao ip
	  empilha(exec, (OPERANDO){NUM, ip});
	  ip = arg.Valor.n;
	  continue;
	case RET:
    // retornar o ip para a posição anterior, que foi empilhada por CALL
	  ip = desempilha(exec).Valor.n;
	  break;
	case EQ:
    // Desempilha os dois valores do topo da pilha e verifica
    // se são iguais
    a = desempilha(pil);
    b = desempilha(pil);
	  if (a.Valor.n == b.Valor.n)
      empilha(pil, (OPERANDO) {BOOL, true});
	  else
      empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case GT:
    // desempilha os dois valores do topo da pilha
    // e verifica se o valor abaixo do topo é maior que o valor no topo
	  if (desempilha(pil).Valor.n < desempilha(pil).Valor.n)
      empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case GE:
    // desempilhar os dois valores no topo e verificar se o valor
    // abaixo do topo da pilha é maior ou igual ao valor do topo
	  if (desempilha(pil).Valor.n <= desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case LT:
  // desempilhar os dois valores que estão no topo da pilha e
  // verificar se o valor abaixo do topo é menor do que o valor no topo
	  if (desempilha(pil).Valor.n > desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case LE:
  // desempilhar os dois valores que estão no topo da pilha e
  // verificar se o valor abaixo do topo é menor ou igual ao
  // que o valor no topo
	  if (desempilha(pil).Valor.n >= desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case NE:
  // desempilhar os dois valores que estão no topo da pilha e
  // verificar se são diferentes
	  if (desempilha(pil).Valor.n != desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case STO:
  // desempilha o valor do topo da pilha e armazena-o no vetor de memória,
  // na posição dada pelo argumento
	  m->Mem[arg.Valor.n] = desempilha(pil);
	  break;
  case RCL:
  // empilha na pilha de dados o valor no vetor de memória na posição dada pelo
  // argumento
	  empilha(pil,m->Mem[arg.Valor.n]);
	  break;
	case END:
    // terminar o programa
	  return;
	case PRN:
    // desempilha e imprime o topo
    a = desempilha(pil);
    if (a.t == CELL)
      printf("Terreno: %d\n Vazio: %d\n Cristais: %d\n Base :%d\n", a.Valor.c.terreno, a.Valor.c.vazia, a.Valor.c.nCristais, a.Valor.c.base);
    else if (a.t == BOOL)
    {
      int b = a.Valor.b;
      if (b == 0)
        printf("false\n");
      else
        printf("true\n");
    } else
      printf("%d\n", a.Valor.n);
    empilha(pil, a);
	  break;
  case STL:
    tmp = desempilha(pil);
    empilha(pil, tmp);
    indexALC = 0;
    while (indexALC != arg.Valor.n + 1){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    empilha(exec, tmp);
    tmp = desempilha(pil);
    indexALC = 0;
    while(indexALC != arg.Valor.n){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    break;
  case RCE:
  //Copia na pilha de dados o valor que está na posição dada pelo argumento
  // na pilha de execução
    indexALC = 0;
    while(indexALC != arg.Valor.n){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    tmp = desempilha(exec);
    empilha(exec, tmp);
    indexALC = 0;
    while (indexALC != arg.Valor.n){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    empilha(pil, tmp);
    break;
  case ALC:
    indexALC = 0;
    while (indexALC != arg.Valor.n)
    {
      empilha(exec,(OPERANDO) {NUM, {0}});
        indexALC++;
    }
    empilha(exec, arg);
    break;
  case FRE:
    indexALC = desempilha(exec).Valor.n;
    while (indexALC != 0)
    {
      tmp = desempilha(exec);
      indexALC--;
    }
    break;
    /* Implementação das novas instruções, sendo elas:
    MOV, ATK, ... (A CONCLUIR) */

  case MOV:
    // DEBUG: Direção correta
    //printf("Moving to %d\n", arg.Valor.d);
    Sistema(0, arg.Valor.ac, m);
    break;
  case ATK:
    Sistema(1, arg.Valor.ac, m);
    break;
  case INF:
    Sistema(2, arg.Valor.ac, m);
    break;
  case CLT:
    Sistema(3, arg.Valor.ac, m);
    break;
  case DEP:
    Sistema(4, arg.Valor.ac, m);
    break;
	case ATR:
		tmp = desempilha(pil);
	   if(tmp.t == CELL){
			if(arg.Valor.n == 0) {
				empilha(pil,(OPERANDO) {NUM, {tmp.Valor.c.terreno}});
			}
			else if(arg.Valor.n == 1) {
				empilha(pil,(OPERANDO) {NUM, {tmp.Valor.c.vazia}});
			}
			else if(arg.Valor.n == 2) {
				empilha(pil,(OPERANDO) {NUM, {tmp.Valor.c.nCristais}});
			}
			else {
				empilha(pil,(OPERANDO) {NUM, {tmp.Valor.c.base}});
			}
		}
		else {
			empilha(pil, tmp);
		}
    break;

	}
	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
