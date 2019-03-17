#ifndef server_framework
#define server_framework

	#include "Definitions.h"
	#include "parse.h"
	#include "s_menu_funct.h"
	#include "s_login_funct.h"
	
	void new_connection(client_list_t *clientList, int socket);
	void remove_connection(client_list_t **clientList, int target_socket);

#endif