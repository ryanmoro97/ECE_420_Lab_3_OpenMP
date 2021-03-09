CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm -lpthread -fopenmp
OBJECTS=main.o Lab3IO.o

all: main datagen serialtester

# Linker:
main: main.o Lab3IO.o
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compilation commands:
Lab3_IO.o: Lab3IO.c Lab3IO.h
	$(CC) $(CFLAGS) -c lab3IO.c -o lab3IO.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -fopenmp
datagen:
	gcc datagen.c Lab3IO.c -o datagen
serialtester:
	gcc serialtester.c Lab3IO.c -o serialtester -lm

clean:
	rm *.o main serialtester datagen data_input data_output
