//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//c_admin_funct.h

#ifndef c_admin_funct
#define c_admin_funct
	
	void admin_login(server_t *server);
	void admin_menu(server_t *server, admin_account_t *admin);
	void ban_user(server_t *server, admin_account_t *admin);
	void kick_user(server_t *server, admin_account_t *admin);
	
#endif
