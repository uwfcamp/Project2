#include "Definitions.h"
#include "c_login_funct.h"
#include "c_menu_funct.h"
#include "c_admin_funct.h"
void admin_login(server_t *server) {
	char username[CREDENTIAL_SIZE];
	char password[CREDENTIAL_SIZE];
	do {
		printf("INPUT ADMIN USERNAME: ");
		fgets(username, CREDENTIAL_SIZE, stdin);
	}while(strlen(username) <=1);
	username[strlen(username)-1]=0;
	do {
		printf("INPUT ADMIN PASSWORD: ");	
		fgets(password, CREDENTIAL_SIZE, stdin);
	}while (strlen(password) <= 1);
	password[strlen(password)-1]=0;	
	sprintf(server->buffer_out, "16%c%s%c%s%c %c ", (char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	server->recieve = 0;
	if(server->is_admin == 1) {
		while(server->is_admin==1) 
			admin_menu(server);
	}
	else
		printf("INVALID USERNAME AND/OR PASSWORD\n");
	return;
}
void admin_menu(server_t *server) {
	char menu_choice[CREDENTIAL_SIZE];
	printf("-=| ADMINISTRATOR |=-\n");
	printf("1. Ban a member\n");
	printf("2. Kick a member\n");
	printf("0. Exit to Main Menu\n");
	do {
		printf("PLEASE MAKE A SELECTION: ");
		fgets(menu_choice, CREDENTIAL_SIZE, stdin);
		menu_choice[strlen(menu_choice)-1]=0;
	}while(atoi(menu_choice) < 0 && atoi(menu_choice) > 2);
	switch(atoi(menu_choice)) {
		case 0://exit case
			printf("\nEXITING ADMIN MENU\n");
			server->is_admin = 0;
			break;
		case 1://ban case
			ban_user(server);
			break;
		case 2://kick case
			kick_user(server);
			break;	
	}
	return;
}
void ban_user(server_t *server) {
	show_all_users(server);
	return;
}

void kick_user(server_t *server) {
	show_all_users(server);
	return;
}
