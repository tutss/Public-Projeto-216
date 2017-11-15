#ifndef MAQ_H
#define MAQ_H
#include "pilha.h"
#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int posx;
  int posy;
  int vida;
  int crist;
  int time;
} Maquina;

// declaração da função cria_maquina
Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV);
// declaração da função destroi_maquina
void destroi_maquina(Maquina *m);
// declaração da função exec_maquina
void exec_maquina(Maquina *m, int n);
// declaração de função para retornar o array de instruções
INSTR* getProg();


void Sistema(int op, int dir,  Maquina *m);
void Atualiza ();
#endif /* MAQ_H*/
