.DEFAULT_GOAL := install

CC = g++
CFLAGS =

install:
	$(CC) $(CFLAGS) rcc.cpp -o rcc

all: install

clean:
	rm -rf rcc