#ifndef client_framework
#define client_framework
	#include "Definitions.h"
	#include "parse.h"
	#include "c_menu_funct.h"
	#include "c_login_funct.h"
	#define IP_ADDRESS "127.0.0.1"

	int menu_input(void);
	int main_menu(server_t *server);

	server_t *build_server_structure(void);
	void disconnect(server_t *server);
	void *server_communication(void *vargp);

#endif