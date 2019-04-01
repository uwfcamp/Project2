//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//client_framework.h

#ifndef client_framework
#define client_framework
	#include "Definitions.h"
	#include "parse.h"
	#include "c_menu_funct.h"
	#include "c_login_funct.h"
	#define IP_ADDRESS "127.0.0.1"

	// will acquire input from the user, and return that input as an integer
	int menu_input(server_t *server);
	// prints the main menu to the terminal, and calls functions based on user input
	int main_menu(server_t *server);
	// will build the server structure for communication with the server
	server_t *build_server_structure(void);
	// will disconnect the client from the server
	void disconnect(server_t *server);
	// thread function, which sends messages to and recieves messages from the server
	void *server_communication(void *vargp);
	// WARNING: ONLY CALL INSIDE OF SERVER_COMMUNICATION THREAD
	// recieves contents of a file that was requested
	void recieve_file(char *body, server_t *server);
	// converts a string to an unsigned long
	unsigned long atoul(char *value);

#endif
