//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.h

#ifndef s_framework
#define s_framework

	pthread_t connectedUsers[MAX_CONNECTED_USERS];
	pthread_t menuThread;
	
	void print_menu(void);
	void * menuThread(void * param);
	void * clientThread(void * param);

	//SERVER GLOBAL VARIABLES
	typedef struct server_s {
		int connected;
		int socket;
		char ip[IP_LENGTH];
		struct server_s *last;
		struct server_s *next;
	}server_t;

#endif
