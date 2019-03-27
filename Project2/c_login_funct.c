#include "Definitions.h"
#include "c_login_funct.h"

int login_menu(server_t *server){
	int selection = -1;

	// print the menu
	printf("\n-=| CHATTERBOX CHAT |=-\n\n");
	printf("1. Registration\n");
	printf("2. Login\n");
	printf("0. Quit\n");
	printf("\n");

	// get the selection
	do{
		selection = menu_input();
	}while(selection<0 || selection>2);

	// perform selection
	switch(selection){
		case 0:
			server->connected=0;
			break;
		case 1:
			registration_input(server);
			break;
		case 2:
			pthread_mutex_lock(&server->lock);
			login_input(server);
			pthread_mutex_unlock(&server->lock);
			break;
	}

	return selection;
}
void registration_input(server_t *server){
	char username[CREDENTIAL_SIZE]={0};
	char password1[CREDENTIAL_SIZE]={0};
	char password2[CREDENTIAL_SIZE]={0};
	int valid=0;

	// print the menu
	printf("\n-=| REGISTER NEW ACCOUNT |=-\n\n");

	do{
		// prompt user for a username
		do {
			printf("USERNAME: ");
			fgets(username, CREDENTIAL_SIZE, stdin);
		} while(strlen(username)<=1);
		username[strlen(username)-1]=0;
		do{
			if (strcmp(password1, password2))
				printf("PASSWORDS DO NOT MATCH\n");
			// prompt user for a password
			do {
				printf("PASSWORD: ");
				fgets(password1, CREDENTIAL_SIZE, stdin);
			}while(strlen(password1)<=1);
			password1[strlen(password1)-1]=0;

			// prompt user to reenter password
			do {
				printf("RE-ENTER PASSWORD: ");
				fgets(password2, CREDENTIAL_SIZE, stdin);
			}while(strlen(password2)<=1);
			password2[strlen(password2)-1]=0;
		}while(strcmp(password1, password2)!=0);

		// send the registration attempt to the server
		sprintf(server->buffer_out,"0%c%s%c%s%c %c ", (char)DELIMITER, username, (char)DELIMITER, password1, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;

		// wait for the response from the server
		while(!server->recieve);

		// check if the server says the user is logged in
		if (server->buffer_in[0] == '0')
			printf("COULD NOT CREATE ACCOUNT, TRY A DIFFERENT USERNAME\n");
		else
			valid=1;

		// let the listening thread know it is okay to read new messages
		server->recieve=0;

	// if the server responds that the user is still attempting
	// to register, then the username must already be in use.
	}while(!valid);
	
	// copy the username and password
	strcpy(server->username, username);
	strcpy(server->password, password1);
	server->logged_in=1;
}

void login_input(server_t *server){
	char username[CREDENTIAL_SIZE]={0};
	char password[CREDENTIAL_SIZE]={0};
	int valid=0;

	// print the menu
	printf("\n-=| LOGIN TO ACCOUNT |=-\n\n");

	do{
		// prompt user for a username
		do {
			printf("USERNAME: ");
			fgets(username, CREDENTIAL_SIZE, stdin);
		} while (strlen(username)<=1);
		username[strlen(username)-1]=0;
		// prompt user for a password
		do {
			printf("PASSWORD: ");
			fgets(password, CREDENTIAL_SIZE, stdin);
		}while (strlen(password)<=1);
		password[strlen(password)-1]=0;


		// send the registration attempt to the server
		sprintf(server->buffer_out,"1%c%s%c%s%c %c ", (char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;

		// wait for the response from the server
		while(!server->recieve);

		// check if the server says the user is logged in
		if (server->buffer_in[0] == '1')
			printf("INVALID LOGIN CREDENTIALS\n");
		
		else {
			char temp[BUFFER_SIZE];
			strcpy(temp,server->buffer_in);
			char * is_banned;
			char search[3];
			search[0]=(char)DELIMITER;
			search[1]='\n';
			search[2]='\0';
			strtok(temp, search);
			strtok(NULL, search);
			strtok(NULL, search);
			strtok(NULL, search);
			is_banned = strtok(NULL, search);
			if (atoi(is_banned) == 1)
				printf("USER IS BANNED\n");
			else
				valid=1;
		}

		// let the listening thread know it is okay to read new messages
		server->recieve=0;

	// if the server responds that the user is still attempting
	// to register, then the username must already be in use.
	}while(!valid);

	// copy the username and password
	strcpy(server->username, username);
	strcpy(server->password, password);
	server->logged_in=1;
}

int is_banned_or_kicked(server_t *server) {
	if(server->is_banned_or_kicked ==0) {
		sprintf(server->buffer_out, "15%c%s%c%s%c %c ",(char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
	}
	return server->is_banned_or_kicked;
}
