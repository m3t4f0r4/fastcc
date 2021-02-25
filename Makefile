# compile fastcc
CC=gcc

fastcc: fastcc.c
	gcc fastcc.c -o fastcc
clean:
	rm fastcc
