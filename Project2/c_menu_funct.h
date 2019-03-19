#ifndef c_menu_funct
#define c_menu_funct
	
	void group_chat(server_t *server);
	void private_chat(server_t *server);
	void logout(server_t *server);	
	void request_users(server_t *server);
	void chat_history(server_t *server);
	void g_chat_history(server_t *server);
	int get_destination(char * destination, server_t *server);

#endif
