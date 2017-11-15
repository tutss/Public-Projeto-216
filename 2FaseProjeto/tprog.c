#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{MOV, ACAO, {NORTH}},
{PRN, NONE, {0}},
{ATK, ACAO, {NORTH}},
{PRN, NONE, {0}},
{INF, NONE, {0}},
{ATR, NUM, {1}},
{PRN, NONE, {0}},
{CLT, ACAO, {SOUTH}},
{PRN, NONE, {0}},
{DEP, ACAO, {SOUTHEAST}},
{PRN, NONE, {0}},
{END, NONE, {0}},
};

int main(int ac, char **av) {
CriaArena(20, 2, 10, 5);
	Maquina *maq = cria_maquina(prog, 5, 5, 100, 0, 1);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
