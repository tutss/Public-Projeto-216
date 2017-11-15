#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH, NUM, {1}},
{DUP, NONE, {0}},
{ADD, NONE, {0}},
{PRN, NONE, {0}},
{PUSH, NUM, {4}},
{DUP, NONE, {0}},
{MUL, NONE, {0}},
{PRN, NONE, {0}},
{END, NONE, {0}},
};

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
