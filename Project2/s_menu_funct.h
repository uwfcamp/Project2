//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_menu_funct.h

#ifndef s_menu_funct
#define s_menu_funct
	
	// send a message to all users, except the one sending the message
	void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender);
	// send a private message to a specific user (destination)
	void private_message(client_list_t *clientList, char *message, char *destination, char *sender);
	// save a message to the group chat log
	void log_into_group(char *username, char *body);
	// send a message to the current client, with a list of all online users
	void show_users(char * username, char * password, client_list_t *clientList, client_list_t *current);
	// send a message to the current client, containing all messages in the group chat log
	void send_group_log(client_list_t *current);
	// send a message to the current client (server), with private messages between them and a designated user (destination)
	void send_private_log(char * destination, client_list_t *server);
	// save a message to the private chat log
	void log_into_private(char *sender, char *reciever, char *body);
	// send a message to the current client, declaring online status of another user (destination)
	void validate_user(char * destination, client_list_t *clientList, client_list_t *current);
	// send a message to the current client, confirming if the designated user (destination) exists
	void confirm_existence(char * destination, client_list_t * current);
	// change the password of the current client, and save it to the logins file
	void change_password(char * body, client_list_t * current);
	// send a message to the current client, listing all registered users
	void show_all_users(client_list_t * current);
	// convert a string to an unsigned long
	unsigned long atoul(char *value);
	// recieve file data from a client
	void recieve_file(char *body, char *destination, client_list_t *current);
	// send a message to the current client with the name of a file and its size, followed by file contents
	void send_file(char *body, client_list_t *current);
	// send a message to the current client with a list of files designated for them
	void show_user_files(client_list_t *current);
	
#endif
