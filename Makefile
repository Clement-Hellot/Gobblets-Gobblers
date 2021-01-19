projet : main.o board.o
	gcc main.o board.o -o projet

main.o: main.c
	gcc -c main.c

board.o: board.c
	gcc -c board.c