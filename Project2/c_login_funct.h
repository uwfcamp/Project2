#ifndef c_login_funct
#define c_login_funct
	#include "Definitions.h"
	#include "client_framework.h"

	int login_menu(server_t *server);
	void registration_input(server_t *server);
	void login_input(server_t *server);
	int is_banned(server_t *server);

#endif
