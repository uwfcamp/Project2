#ifndef s_admin_funct
#define s_admin_funct
	void admin_login(char * username, char * password, client_list_t *current, admin_account_t * admin);
	void ban_user(char * destination, client_list_t * clientList, client_list_t * current);
	void kick_user(char * destination, client_list_t * clientList, client_list_t * current);
#endif
