//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.h

#include <pthread.h>


#ifndef s_framework
#define s_framework

	//SERVER GLOBAL VARIABLES
	typedef struct server_s {
		int force_close;
		int connected;
		int socket;
		pthread_t TID;
		struct server_s *last;
		struct server_s *next;
	}server_t;

	void print_menu(void);
	int get_menu_option(char * userInput);
	void * menuThread(void * param);
	void * clientThread(void * param);
	server_t *clientTerminate(server_t *clientList, int socket);
	server_t *clientCreate(server_t *clientList, int socket);
	void printClientCount(server_t *clientList);
	long long get_file_size(char *filename);
#endif
