//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_admin_funct.h

#ifndef s_admin_funct
#define s_admin_funct

	// will verify the adminstrative credentials from the client, and let them know if they are valid
	void admin_login(char * username, char * password, client_list_t *current, admin_account_t * admin);
	// will ban a user from the server, and let them know that they are banned
	void ban_user(char * destination, client_list_t * clientList, client_list_t * current);
	// will kick a user off the server, and let them know that they are kicked out
	void kick_user(char * destination, client_list_t * clientList, client_list_t * current);

#endif
