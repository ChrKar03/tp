CC:=gcc
CFLAGS:=-Wall -fsanitize=address -g
LIBS:=-lpthread
SRC:=merge.c
TARGET:= merge

build: clear
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS) 

.PHONY:
clear:
	rm -rf $(TARGET)