//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_login_funct.h

#ifndef s_login_funct
#define s_login_funct
	
	int login_user(char *username, char *password, client_list_t *client);
	int register_user(char *username, char *password, client_list_t *client);
	void s_logout(char * username,char * password, client_list_t *client);
	void check_if_banned(char * username, client_list_t *current);

#endif
