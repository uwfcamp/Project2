//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_login_funct.h

#ifndef s_login_funct
#define s_login_funct
	
	// will verify the client's username and password, and send them a message saying if they are logged in or not
	int login_user(char *username, char *password, client_list_t *client);
	// will register a new user, provided they are not giving a username that is already in use
	int register_user(char *username, char *password, client_list_t *client);
	// will log the user out of the server, and let the client know
	void s_logout(char * username,char * password, client_list_t *client);
	// will check if the user is banned, and let the client know
	void check_if_banned(char * username, client_list_t *current);

#endif
