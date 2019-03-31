//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//server_framework.h

#ifndef server_framework
#define server_framework

	#include "Definitions.h"
	#include "parse.h"
	#include "s_menu_funct.h"
	#include "s_login_funct.h"
	
	// add a new connection to the link list, located at the designated socket
	void new_connection(client_list_t *clientList, int socket);
	// remove the connection located at the designated socket
	void remove_connection(client_list_t *clientList, int target_socket);
	// thread function to let the main thread know to send a message twice every TIMEOUT_INTERVAL
	void *ping_connections(void *vargp);

#endif
