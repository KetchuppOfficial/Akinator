CC = gcc
CFLAGS = -c -Wall -Werror -Wextra

TREE_FILE = Tree.txt
DOT_FILE  = Graphic_Dump/Tree.dot

all: Akinator

Akinator: main.o Akinator.o Stack.o
	$(CC) Objects/main.o Objects/Akinator.o Objects/Stack.o ../../My_Lib/My_Lib.a -o Akinator.out
	rm Objects/main.o
	rm Objects/Akinator.o
	rm Objects/Stack.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -o Objects/main.o

Akinator.o: Akinator.c
	$(CC) $(CFLAGS) Akinator.c -o Objects/Akinator.o

Stack.o: Stack/Stack.c
	$(CC) $(CFLAGS) Stack/Stack.c -o Objects/Stack.o

run:
	./Akinator.out $(TREE_FILE) $(DOT_FILE)

clean:
	rm -f Objects/main.o Objects/Akinator.o Objects/Stack.o
	rm -f log_file.log
	rm Akinator.out
