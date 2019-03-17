#ifndef s_menu_funct
#define s_menu_funct
	
	void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender);
	void private_message(client_list_t *clientList, char *message, char *destination, char *sender);
	void log_into_group(char *username, char *body);

#endif