CC = gcc
CFLAGS = -g -Wall
LINKS = -pthread

OBJECTS1 = server_framework.o
OBJECTS2 = client_framework.o
OBJECTS3 = parse.o
CFILES1 = server_framework.c server_framework.h parse.h parse.c
CFILES2 = client_framework.c client_framework.h parse.h parse.c
CFILES3 = parse.c parse.h Definitions.h
BINARYS = server client

all: $(BINARYS)

server: $(OBJECTS1)
	$(CC) $(LINKS) -o server $(OBJECTS1) $(OBJECTS3)
	
client: $(OBJECTS2)
	$(CC) $(LINKS) -o client $(OBJECTS2) $(OBJECTS3)

client_framework.o: $(CFILES1)
	$(CC) -c $(CFLAGS) client_framework.c
server_framework.o: $(CFILES2)
	$(CC) -c $(CFLAGS) server_framework.c
	
.PHONY : clean
clean:
rm $(BINARYS) $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)
