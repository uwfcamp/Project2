//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//c_admin_funct.h

#ifndef c_admin_funct
#define c_admin_funct
	
	// will promt the user for administrator credentials
	void admin_login(server_t *server);

	// will print a menu of administrative options, and allow the user to choose one
	void admin_menu(server_t *server, admin_account_t *admin);

	// will prompt the user for the name of another user, and ban that user from the chat
	void ban_user(server_t *server, admin_account_t *admin);

	// will prompt the user for the name of another user, and kick that user out
	void kick_user(server_t *server, admin_account_t *admin);
	
#endif
