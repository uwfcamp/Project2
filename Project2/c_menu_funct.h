//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//c_menu_funct.h

#ifndef c_menu_funct
#define c_menu_funct
	
	// perform group chat, allowing user to send and recieve messages in the group chat
	void group_chat(server_t *server);
	// perform private chat, allowing user to recieve private messages, and send messages to specific user
	void private_chat(server_t *server);
	// send server a logout request, and wait for response
	void logout(server_t *server);
	// ask the server for a list of all online users, and print it to the terminal
	void request_users(server_t *server);
	// allow the user to choose between viewing group chat history, or private chat history
	void chat_history(server_t *server);
	// request the group chat history from the server, and print it out once recieved
	void g_chat_history(server_t *server);
	// prompt the user for another username, and print all messages between the user and designated other
	void p_chat_history(server_t *server);
	// ask the user for a destination username, and confirm it is valid
	int get_destination(char * destination, server_t *server);
	// prompt the user for the old password and a new one, and ask the server to change the password
	void change_password(server_t *server);
	// ask the server for a list of all registered users, and print it to the terminal
	void show_all_users(server_t *server);
	// send a file to the server
	void send_file(server_t *server);
	// prompt user for the name of a file, and ensure the file is valid and accessible
	int get_file_name(char *filename);
	// get the size of a designated file, and return it as an unsigned long
	unsigned long get_file_size(char *filename);
	// print a menu with file transfer options, and ask the user to choose one
	void file_menu(server_t *server);
	// recieve file data from the server
	void retrieve_file(server_t *server);

#endif
