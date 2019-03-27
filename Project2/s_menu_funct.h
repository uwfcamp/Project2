#ifndef s_menu_funct
#define s_menu_funct
	
	void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender);
	void private_message(client_list_t *clientList, char *message, char *destination, char *sender);
	void log_into_group(char *username, char *body);
	void show_users(char * username, char * password, client_list_t *clientList, client_list_t *current);
	void send_group_log(client_list_t *current);
	void send_private_log(char * destination, client_list_t *server);
	void log_into_private(char *sender, char *reciever, char *body);
	void validate_user(char * destination, client_list_t *clientList, client_list_t *current);
	void confirm_existence(char * destination, client_list_t * current);
	void change_password(char * body, client_list_t * current);
	void show_all_users(client_list_t * current);
	unsigned long atoul(char *value);
	void recieve_file(char *body, char *destination, client_list_t *current);
	
#endif
