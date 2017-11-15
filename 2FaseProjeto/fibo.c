#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH, NUM, {1}},
{DUP, NONE, {0}},
{STO, NONE, {0}},
{STO, NONE, {1}},
{PUSH, NUM, {10}},
{STO, NONE, {2}},
{RCL, NONE, {0}},
{RCL, NONE, {1}},
{DUP, NONE, {0}},
{STO, NONE, {0}},
{ADD, NONE, {0}},
{DUP, NONE, {0}},
{STO, NONE, {1}},
{PRN, NONE, {0}},
{RCL, NONE, {2}},
{PUSH, NUM, {1}},
{SUB, NONE, {0}},
{DUP, NONE, {0}},
{STO, NONE, {2}},
{PUSH, NUM, {0}},
{EQ, NONE, {0}},
{JIF, NONE, {6}},
{END, NONE, {0}},
};

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
