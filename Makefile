CC=gcc
CFLAGS=-Wall 
SSL=-lssl -lcrypto

main: main.c
	$(CC) $(CFLAGS) $< -o $@ $(SSL)

