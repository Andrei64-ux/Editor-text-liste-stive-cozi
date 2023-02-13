CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g
SOURCES = main.c Doubly_linked_list.c Stack.c Node.c 
EXE = editor

build:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXE)
clean:
	rm -f $(EXE)