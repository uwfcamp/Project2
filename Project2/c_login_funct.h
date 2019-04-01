//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//c_login_funct.h

#ifndef c_login_funct
#define c_login_funct
	#include "Definitions.h"
	#include "client_framework.h"

	// print the initial login menu, and allow the user to choose what they want to do
	int login_menu(server_t *server);
	// promt the user for a username and password, and send them to the server for registration
	void registration_input(server_t *server);
	// promt the user for a username and password, and send them to the server to validate
	void login_input(server_t *server);
	// send a mssage to the server to ask if the user is banned or kicked out
	int is_banned_or_kicked(server_t *server);

#endif
