CC = gcc
CFLAGS = -g -Wall
LINKS = -pthread

OBJECTS1 = s_framework.o
OBJECTS2 = c_framework.o

CFILES1 = s_framework.c s_framework.h 
CFILES2 = c_framework.c c_framework.h
BINARYS = server client

all: $(BINARYS)

server: $(OBJECTS1)
	$(CC) $(LINKS) -o server $(OBJECTS1)
client: $(OBJECTS2) $(OBJECTS3)
	$(CC) $(LINKS) -o client $(OBJECTS2)

s_framework.o: $(CFILES1)
	$(CC) -c $(CFLAGS) s_framework.c
c_framework.o: $(CFILES2)
	$(CC) -c $(CFLAGS) c_framework.c
	
.PHONY : clean
clean:
	rm $(BINARYS) $(OBJECTS1) $(OBJECTS2)
