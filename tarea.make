CC=gcc
DEBUG = -g
FLAGS = -Wall

default:exe

exe: main.o funciones.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ -c $(FLAGS)

clean:
	rm *.o exe

run:
	./exe