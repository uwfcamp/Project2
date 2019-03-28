#include "Definitions.h"
#include "c_login_funct.h"
#include "c_menu_funct.h"
#include "c_admin_funct.h"
/*****************************************
 * This function is the queries the server with the submitted
 * admin username and password. If successful, the program will
 * transition into the admin_menu function. If unsuccessful,
 * the program will notify the user and return to main menu.
 * ***************************************/
void admin_login(server_t *server) {
	admin_account_t *admin= (admin_account_t *) malloc(sizeof(admin_account_t));
	clear_string(admin->username, CREDENTIAL_SIZE);
	clear_string(admin->password, CREDENTIAL_SIZE);
	// accept username from user. Will continue prompting until user enters something
	while(strlen(admin->username) <1 && server->is_banned_or_kicked == 0) {
		printf("INPUT ADMIN USERNAME: ");
		fgets(admin->username, CREDENTIAL_SIZE, stdin);
		admin->username[strlen(admin->username)-1]=0;
	}
	//accept password from user. Will continue prompting until user enters something
	while(strlen(admin->password) < 1 && server->is_banned_or_kicked == 0) {
		printf("INPUT ADMIN PASSWORD: ");	
		fgets(admin->password, CREDENTIAL_SIZE, stdin);
		admin->password[strlen(admin->password)-1]=0;	
	}
	//send check server for valid admin credentials
	if(server->is_banned_or_kicked ==0) {
		sprintf(server->buffer_out, "16%c%s%c%s%c %c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
		server->recieve = 0;
	}
	// if successful move into the admin menu and continue until admin
	// exits menu
	if(server->is_admin == 1 && server->is_banned_or_kicked==0) {
		while(server->is_admin==1 && server->is_banned_or_kicked == 0) 
			admin_menu(server, admin);
	}
	// if unsuccessful, notify user invalid credentials and exit to
	// main menu.
	else if(server->is_banned_or_kicked==0)
		printf("INVALID USERNAME AND/OR PASSWORD\n");
	free(admin);
	return;
}
/**************************************************************
 * Admin menu will prompt user to select menu option or to exit
 * the menu.
 * ***********************************************************/
void admin_menu(server_t *server, admin_account_t *admin) {
	char menu_choice[CREDENTIAL_SIZE];
	clear_string(menu_choice, CREDENTIAL_SIZE);
	printf("-=| ADMINISTRATOR |=-\n");
	printf("1. Ban a member\n");
	printf("2. Kick a member\n");
	printf("0. Exit to Main Menu\n");
	//get menu selection from user
	while((strlen(menu_choice) < 1 || (atoi(menu_choice) < 0  && atoi(menu_choice) > 2)) && server->is_banned_or_kicked==0) {
		printf("PLEASE MAKE A SELECTION: ");
		fgets(menu_choice, CREDENTIAL_SIZE, stdin);
		menu_choice[strlen(menu_choice)-1]=0;
	}
	//move to switch case for the menu option
	if(server->is_banned_or_kicked==0) {
		switch(atoi(menu_choice)) {
			case 0://exit case
				printf("\nEXITING ADMIN MENU\n");
				server->is_admin = 0;
				break;
			case 1://ban case
				ban_user(server, admin);
				break;
			case 2://kick case
				kick_user(server, admin);
				break;	
		}
	}
	return;
}

/******************************************************
 * This function allows the admin to ban a user, it will
 * print out the names of all existing users to help the
 * admin see who all they can ban. The admin will enter 
 * the name of a valid user or use the _q exit case to
 * abort the banning of users. After a valid username is
 * entered, the server will handle the ban case.
 * ***************************************************/
void ban_user(server_t *server, admin_account_t *admin) {
	char input[CREDENTIAL_SIZE];
	server->valid_destination = 0;
	printf("-=| BAN USER |=-\n");
	//show list of valid users then prompt user for name of user to ban
	do {
		show_all_users(server);
		do {
			printf("ENTER NAME OF USER TO BAN OR _q TO ABORT: ");
			fgets(input, CREDENTIAL_SIZE, stdin);
			input[strlen(input)-1]=0;
		}while (strlen(input)<1);
		if (!strcmp(input, "_q"))
			return;	
		//set up buffer and send to server to see if inputted username is valid
		sprintf(server->buffer_out, "14%c%s%c%s%c%s%c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, input, (char)DELIMITER);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
		//if username is not valid, tell user that name is not valid
		if(!server->valid_destination)
			printf("%s DOES NOT EXIST\n", input);
	}while(!server->valid_destination);
	//if username is valid, send name to the server so that user is banned
	sprintf(server->buffer_out, "11%c%s%c%s%c%s%c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, input, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	return;
}

void kick_user(server_t *server, admin_account_t *admin) {
	server->valid_destination=0;
	char input[BUFFER_SIZE];
	printf("-=| KICK USER |=-\n");
	do {
		request_users(server);
		do {
			printf("ENTER NAME OF USER TO KICK OR _q TO ABORT: ");
			fgets(input, BUFFER_SIZE, stdin);
			input[strlen(input)-1]=0;
		}while(strlen(input)<1);
		if (!strcmp(input, "_q"))
			return;
		sprintf(server->buffer_out, "13%c%s%c%s%c%s%c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, input, (char)DELIMITER);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
		if(!server->valid_destination)
			printf("%s DOES NOT EXIST\n", input);
	}while(!server->valid_destination);
	sprintf(server->buffer_out, "12%c%s%c%s%c%s%c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, input, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	return;
}
