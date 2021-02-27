CC=gcc
CFLAGS=-Wall -O3
LDFLAGS=-lm -lpthread
OBJECTS=main.o lab3IO.o

all: main

# Linker:
main: main.o lab3IO.o
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compilation commands:
lab3_IO.o: lab3IO.c lab3IO.h
	$(CC) $(CFLAGS) -c lab3IO.c -o lab3IO.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o main
