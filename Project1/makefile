CC = gcc
CFLAGS = -g -Wall
LINKS = 

OBJECTS1 = server.o
OBJECTS2 = client.o
CFILES1 = server.c
CFILES2 = client.c
BINARYS = tcpS tcpC

all: $(BINARYS)

tcpS: $(OBJECTS1)
	$(CC) $(LINKS) -o tcpS $(OBJECTS1)
	
tcpC: $(OBJECTS2)
	$(CC) $(LINKS) -o tcpC $(OBJECTS2)

server.o: $(CFILES1)
	$(CC) -c $(CFLAGS) server.c
client.o: $(CFILES2)
	$(CC) -c $(CFLAGS) client.c
	
.PHONY : clean
clean:
	rm $(BINARYS) $(OBJECTS1) $(OBJECTS2)
