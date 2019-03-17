CC = gcc
CFLAGS = -g -Wall
LINKS = -pthread

OBJECTS1 = server_framework.o
OBJECTS2 = client_framework.o c_menu_funct.o c_login_funct.o
OBJECTS3 = parse.o

CFILES1 = server_framework.c
CFILES2 = client_framework.c client_framework.h c_login_funct.c c_login_funct.h c_menu_funct.c c_menu_funct.h 
CFILES3 = parse.c parse.h Definitions.h
BINARYS = server client

all: $(BINARYS)

server: $(OBJECTS1) $(OBJECTS3)
	$(CC) $(LINKS) -o server $(OBJECTS1) $(OBJECTS3)
client: $(OBJECTS2) $(OBJECTS3)
	$(CC) $(LINKS) -o client $(OBJECTS2) $(OBJECTS3)

server_framework.o: $(CFILES1) $(CFILES3)
	$(CC) -c $(CFLAGS) server_framework.c
client_framework.o: $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) client_framework.c
c_login_funct.o:  $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) c_login_funct.c
c_menu_funct.o:  $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) c_menu_funct.c
parse.o: $(CFILES3)
	$(CC) -c $(CFLAGS) parse.c
	
.PHONY : clean
clean:
	rm $(BINARYS) $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)
