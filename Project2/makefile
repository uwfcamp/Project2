CC = gcc
CFLAGS = -g -Wall
LINKS = -pthread

OBJECTS1 = client.o
OBJECTS2 = lizardsUni.o
CFILES1 = lizards.c
CFILES2 = lizardsUni.c
BINARYS = lizards lizardsUni

all: $(BINARYS)

lizards: $(OBJECTS1)
	$(CC) $(LINKS) -o lizards $(OBJECTS1)
	
lizardsUni: $(OBJECTS2)
	$(CC) $(LINKS) -o lizardsUni $(OBJECTS2)

lizards.o: $(CFILES1)
	$(CC) -c $(CFLAGS) lizards.c
lizardsUni.o: $(CFILES2)
	$(CC) -c $(CFLAGS) lizardsUni.c
	
.PHONY : clean
clean:
rm $(BINARYS) $(OBJECTS1) $(OBJECTS2)
