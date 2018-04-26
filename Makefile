CC=gcc

default:exe

exe: main.o functions.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ -c 

run:
	./exe
clean:
	rm -rf main.o functions.o