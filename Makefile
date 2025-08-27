CC=gcc
CFLAGS=-I./src -Wall
SRC=src/main.c src/auth.c src/system.c
OUT=atm-management-system.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	del $(OUT)
