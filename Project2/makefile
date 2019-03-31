CC = gcc
CFLAGS = -g -Wall
LINKS = -pthread

OBJECTS1 = server_framework.o s_menu_funct.o s_login_funct.o s_admin_funct.o
OBJECTS2 = client_framework.o c_menu_funct.o c_login_funct.o c_admin_funct.o
OBJECTS3 = parse.o

CFILES1 = server_framework.c server_framework.c s_login_funct.c s_login_funct.h s_menu_funct.c s_menu_funct.h s_admin_funct.h s_admin_funct.c
CFILES2 = client_framework.c client_framework.h c_login_funct.c c_login_funct.h c_menu_funct.c c_menu_funct.h c_admin_funct.h c_admin_funct.c
CFILES3 = parse.c parse.h Definitions.h
BINARYS = server client
TXTFILES = logins.txt privatechat.txt groupchat.txt filename.txt

all: $(BINARYS)

server: $(OBJECTS1) $(OBJECTS3)
	$(CC) $(LINKS) -o server $(OBJECTS1) $(OBJECTS3)
client: $(OBJECTS2) $(OBJECTS3)
	$(CC) $(LINKS) -o client $(OBJECTS2) $(OBJECTS3)

server_framework.o: $(CFILES1) $(CFILES3)
	$(CC) -c $(CFLAGS) server_framework.c
client_framework.o: $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) client_framework.c
s_login_funct.o:  $(CFILES1) $(CFILES3)
	$(CC) -c $(CFLAGS) s_login_funct.c
s_menu_funct.o:  $(CFILES1) $(CFILES3)
	$(CC) -c $(CFLAGS) s_menu_funct.c
s_admin_funct.o: $(CFILES1) $(CFILES3)
	$(CC) -c $(CFLAGS) s_admin_funct.c
c_login_funct.o:  $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) c_login_funct.c
c_menu_funct.o:  $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) c_menu_funct.c
c_admin_funct.o: $(CFILES2) $(CFILES3)
	$(CC) -c $(CFLAGS) c_admin_funct.c
parse.o: $(CFILES3)
	$(CC) -c $(CFLAGS) parse.c
	
.PHONY : clean cleaner
clean:
	rm $(BINARYS) $(OBJECTS1) $(OBJECTS2) $(OBJECTS3)

cleaner: 
	rm $(TXTFILES)
