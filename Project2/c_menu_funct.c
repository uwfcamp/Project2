#include "Definitions.h"
#include "c_menu_funct.h"
#include "parse.h"

void group_chat(server_t *server){
	char input;
	printf("\n-=| GROUP CHAT |=-");
	printf("\n-=| HIT ENTER TO TYPE A MESSAGE |=-");
	printf("\n-=| PRESS Q THEN ENTER TO EXIT |=-\n\n");
	server->in_group_chat=1;
	do{
		input=getchar();
		if (input!='Q' && input!='q'){
			while(server->send==1);
			// mutex 1 lock to replace typing variable
			server->typing=1;
			printf("YOUR MESSAGE: ");
			sprintf(server->buffer_out,"7%c%s%c%s%c %c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
			fgets(&server->buffer_out[strlen(server->buffer_out)], BUFFER_SIZE-strlen(server->buffer_out), stdin);
			server->buffered_out_size=strlen(server->buffer_out)+1;
			server->send=1;
			server->typing=0;
			// mutex 1 unlock to replace typing variable
		}

		if (input!='\n')
			while(getchar()!='\n');
	}while(input != 'q' && input !='Q');
	server->in_group_chat=0;
}



/*
************************************MY EDITS *******************************************
**************************client side private message function*************************
*/
void private_chat(server_t *server){
	char input;
	char destination[CREDENTIAL_SIZE]={0};
	int p_exit = -1;

	printf("\n-=|            PRIVATE CHAT             |=-");
	printf("\n-=| HIT ENTER TO SEND A PRIVATE MESSAGE |=-");
	printf("\n-=|     PRESS Q THEN ENTER TO EXIT      |=-\n\n");
	server->in_private_chat=1;
	do{
		input=getchar();
		if (input!='Q' && input!='q'){
			while(server->send==1);
			p_exit = get_destination(destination, server);
			// mutex 1 lock to replace typing variable
			if(p_exit != 1) {
				server->typing=1;
				printf("YOUR MESSAGE: ");
				sprintf(server->buffer_out,"6%c%s%c%s%c%s%c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
				fgets(&server->buffer_out[strlen(server->buffer_out)], BUFFER_SIZE-strlen(server->buffer_out), stdin);
				server->buffered_out_size=strlen(server->buffer_out)+1;
				server->typing=0;
				server->send=1;
				while(server->send==1);
			}
			// mutex 1 unlock to replace typing variable
		}

		if (input!='\n')
			while(getchar()!='\n');
	}while(input != 'q' && input !='Q');
	server->in_private_chat=0;
}
//******************************************************************************************

void logout(server_t *server) {
	sprintf(server->buffer_out, "3%c%s%c%s%c %c " , (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	printf("logging out\n");
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;	
	server->typing=0;
	return;
}



void request_users(server_t *server){
	while(server->send==1);
	sprintf(server->buffer_out, "5%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	while(server->recieve!=2);
	server->recieve=0;
}
void chat_history(server_t *server) {
	int menuChoice = -1;
	while (menuChoice != 0){
		printf("\n-=| CHAT HISTORY |=-");
		printf("\n1. Group Chat");
		printf("\n2. Private Chat");
		printf("\n0. QUIT");
		printf("\n\nENTER SELECTION: ");
		fflush(stdin);
		scanf("%d", &menuChoice);
		fflush(stdin);
		if(menuChoice == 1)
			g_chat_history(server);
	}
	return;
}
void g_chat_history(server_t *server){
	while(server->send==1);
	printf("\n-=| Group Chat History |=-");
	sprintf(server->buffer_out, "8%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	while(server->recieve!=2);
	server->recieve=0;
	return;
}
int get_destination(char * destination, server_t *server) {
	server->valid_destination = 0;
	do{
		// prompt user for a destination
		printf("Enter \\q to abort\n");
		printf("DESTINATION: ");
		fflush(stdin);
		fgets(destination, CREDENTIAL_SIZE, stdin);
		destination[strlen(destination)-1]='\0';
		if (strcmp(destination, "\\n")==0) {
			return 1;
		}
		// query the server to validate the desination QUERYUSER is a placeholder for a new request mode integer
		sprintf(server->buffer_out,"13%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;

		// wait for the response from the server
		while(server->recieve != 2);

		// check if the server says the destination is valid - Here i'm just reusing the login successful bit but a new response field may be defined as needed
		if (server->valid_destination==0)
			printf("INVALID TARGET\n");
		// let the listening thread know it is okay to read new messages
		server->recieve=0;

	// if the server responds that the user is still attempting
	// to register, then the username must already be in use.
	}while(!server->valid_destination);
	return 0;
}
