//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//c_menu_funct.h

#ifndef c_menu_funct
#define c_menu_funct
	
	void group_chat(server_t *server);
	void private_chat(server_t *server);
	void logout(server_t *server);	
	void request_users(server_t *server);
	void chat_history(server_t *server);
	void g_chat_history(server_t *server);
	void p_chat_history(server_t *server);
	int get_destination(char * destination, server_t *server);
	void change_password(server_t *server);
	void show_all_users(server_t *server);
	void send_file(server_t *server);
	int get_file_name(char *filename);
	unsigned long get_file_size(char *filename);
	void file_menu(server_t *server);
	void retrieve_file(server_t *server);

#endif
