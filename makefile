mainA: mainA.c
	gcc mainA.c -o MAINA -lpthread -ansi -pedantic

mainB: mainB.c
	gcc mainB.c -o MAINB -lpthread -ansi -pedantic
