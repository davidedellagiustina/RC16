.DEFAULT_GOAL := install

CC = g++
CFLAGS = -g -O3 -mtune=native -march=native

install:
	$(CC) $(CFLAGS) rcc.cpp -o rcc

all: install

clean:
	rm -rf rcc