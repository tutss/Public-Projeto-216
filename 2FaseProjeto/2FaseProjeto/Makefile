CC = gcc
CFLAGS = -g -w
default: arena

arena: maq.o pilha.o arena.o tprog.o
	$(CC) $(CFLAGS) maq.o pilha.o arena.o tprog.o -o arena

maq.o: maq.c maq.h pilha.h instr.h
	$(CC) $(CFLAGS) -c maq.c

tprog.o: tprog.c maq.h pilha.h instr.h
	$(CC) $(CFLAGS) -c tprog.c

pilha.o: pilha.c pilha.h instr.h util.h
	$(CC) $(CFLAGS) -c pilha.c

arena.o: arena.c arena.h
	$(CC) $(CFLAGS) -c arena.c

clean:
	rm -f arena *.o
