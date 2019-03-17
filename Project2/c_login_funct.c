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
			login_input(server);
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
		printf("USERNAME: ");
		scanf(" %s", username);
		while(getchar()!='\n');
		do{
			if (strcmp(password1, password2))
				printf("PASSWORDS DO NOT MATCH\n");
			// prompt user for a password
			printf("PASSWORD: ");
			scanf(" %s", password1);
			while(getchar()!='\n');

			// prompt user to reenter password
			printf("RE-ENTER PASSWORD: ");
			scanf(" %s", password2);
			while(getchar()!='\n');
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
		printf("USERNAME: ");
		scanf(" %s", username);
		while(getchar()!='\n');

		// prompt user for a password
		printf("PASSWORD: ");
		scanf(" %s", password);
		while(getchar()!='\n');

		// send the registration attempt to the server
		sprintf(server->buffer_out,"1%c%s%c%s%c %c ", (char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;

		// wait for the response from the server
		while(!server->recieve);

		// check if the server says the user is logged in
		if (server->buffer_in[0] == '1')
			printf("INVALID LOGIN CREDENTIALS\n");
		else
			valid=1;

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
