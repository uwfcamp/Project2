//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.h

#ifndef s_framework
#define s_framework

	void print_menu(void);

	//SERVER GLOBAL VARIABLES
	typedef struct server_s {
		int connected;
		int socket;
		char ip[IP_LENGTH];
		struct server_s *last;
		struct server_s *next;
	}server_t;
#endif
