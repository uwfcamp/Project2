#include "Definitions.h"
#include "c_login_funct.h"
#include "c_menu_funct.h"
#include "c_admin_funct.h"
void admin_login(server_t *server) {
	admin_account_t *admin= (admin_account_t *) malloc(sizeof(admin_account_t));
	do {
		printf("INPUT ADMIN USERNAME: ");
		fgets(admin->username, CREDENTIAL_SIZE, stdin);
	}while(strlen(admin->username) <=1);
	admin->username[strlen(admin->username)-1]=0;
	do {
		printf("INPUT ADMIN PASSWORD: ");	
		fgets(admin->password, CREDENTIAL_SIZE, stdin);
	}while (strlen(admin->password) <= 1);
	admin->password[strlen(admin->password)-1]=0;	
	sprintf(server->buffer_out, "16%c%s%c%s%c %c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	server->recieve = 0;
	if(server->is_admin == 1) {
		while(server->is_admin==1) 
			admin_menu(server, admin);
	}
	else
		printf("INVALID USERNAME AND/OR PASSWORD\n");
	free(admin);
	return;
}
void admin_menu(server_t *server, admin_account_t *admin) {
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
			ban_user(server, admin);
			break;
		case 2://kick case
			kick_user(server, admin);
			break;	
	}
	return;
}
void ban_user(server_t *server, admin_account_t *admin) {
	char input[CREDENTIAL_SIZE];
	server->valid_destination = 0;
	printf("-=| BAN USER |=-\n");
	do {
		show_all_users(server);
		do {
			printf("ENTER NAME OF USER TO BAN OR _q TO ABORT: ");
			fgets(input, CREDENTIAL_SIZE, stdin);
			input[strlen(input)-1]=0;
		}while (strlen(input)<1);
		if (!strcmp(input, "_q"))
			return;	
		sprintf(server->buffer_out, "14%c%s%c%s%c%s%c ", (char)DELIMITER, admin->username, (char)DELIMITER, admin->password, (char)DELIMITER, input, (char)DELIMITER);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
		if(!server->valid_destination)
			printf("%s DOES NOT EXIST\n", input);
	}while(!server->valid_destination);
	printf("%s WAS BANNED\n", input);
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
	printf("%s WAS KICKED\n", input);
	return;
}
