#include "Definitions.h"
#include "c_menu_funct.h"
#include "parse.h"

/*
**************************client side public message function*************************
*/
void group_chat(server_t *server){
	char input[BUFFER_SIZE];
	server->in_group_chat=1;
	do{
		char message[BUFFER_SIZE-strlen(server->buffer_out)];
		printf("\n-=| GROUP CHAT |=-");
		printf("\n-=| HIT ENTER TO TYPE A MESSAGE |=-");
		printf("\n-=| PRESS Q THEN ENTER TO EXIT |=-\n\n");
		fgets(input, BUFFER_SIZE, stdin);
		
		if (input[0] == '\n') {	//if the enter key was pressed
			while(server->send==1);
			// preload header to buffer_out
			sprintf(server->buffer_out,"7%c%s%c%s%c %c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
			// mutex 1 lock to replace typing variable
			server->typing=1;
			//read user input untill the message exists and return has been hit
			do {
				printf("Enter _q to abort\n");
				printf("YOUR MESSAGE: ");
				fgets(message, BUFFER_SIZE-strlen(server->buffer_out), stdin);
				fflush(stdin);
				if(strlen(message)<=1)
					printf("MESSAGE CANNOT BE NULL\n");
			}while(strlen(message)<=1);
			//if message is "_q" then cancel message
			if (strcmp(input, "_q\n")) {
				strcat(server->buffer_out, message);
				server->buffered_out_size=strlen(server->buffer_out)+1;
				server->send=1;
			}
			server->typing=0;
			// mutex 1 unlock to replace typing variable
		}

	}while((strlen(input) != 2) || ((input[0] != 'q') && (input[0] !='Q')));
	server->in_group_chat=0;
}



/*
**************************client side private message function*************************
*/
void private_chat(server_t *server){
	char input[BUFFER_SIZE];
	char destination[CREDENTIAL_SIZE]={0};
	int dest_valid = -1;// 0=target valid, 1=target invalid
// set system variable
	server->in_private_chat=1;
	do{
		printf("\n-=|            PRIVATE CHAT             |=-");
		printf("\n-=| HIT ENTER TO SEND A PRIVATE MESSAGE |=-");
		printf("\n-=|     PRESS Q THEN ENTER TO EXIT      |=-\n\n");
		// read user input
		fgets(input, BUFFER_SIZE, stdin);
		// if input is a null string get user message
		if (input[0]=='\n'){
			while(server->send==1);
			// 
			dest_valid = get_destination(destination, server);
			if(dest_valid == 0) {	//changed from (dest_valid != 1) to prevent -1 from passing
				server->typing=1;// mutex 1 lock to replace typing variable
				//preload header info into server message
				sprintf(server->buffer_out,"6%c%s%c%s%c%s%c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
					char message[BUFFER_SIZE-strlen(server->buffer_out)];
				do {
					printf("Enter _q to abort\nYOUR MESSAGE: ");
					fgets(message, BUFFER_SIZE-strlen(server->buffer_out), stdin);
					if(strlen(message)<=1){
						printf("MESSAGE CANNOT BE NULL\n");
					}
				}while(strlen(message)<=1);
				if (strcmp(message, "_q\n")) {
					strcat(server->buffer_out, message);	
					server->buffered_out_size=strlen(server->buffer_out)+1;
					server->typing=0;
					server->send=1;
					while(server->send==1);
				}
			}
			// mutex 1 unlock to replace typing variable
		}

	}while((strlen(input)!=2) || (input[0] != 'q' && input[0] !='Q'));
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
	sem_wait(&server->mutex);
	server->recieve=0;
}
void chat_history(server_t *server) {
	char menuChoice[CREDENTIAL_SIZE];
	while (menuChoice != 0){
		printf("\n-=| CHAT HISTORY |=-");
		printf("\n1. Group Chat");
		printf("\n2. Private Chat");
		printf("\n0. QUIT");
		printf("\n\nENTER SELECTION: ");
		fflush(stdin);
		fgets(menuChoice, CREDENTIAL_SIZE, stdin);
		menuChoice[strlen(menuChoice)-1]='\0';
		if(atoi(menuChoice) == 1)
			g_chat_history(server);
		else if(atoi(menuChoice) == 2)
			p_chat_history(server);
		else if(atoi(menuChoice) == 0)
			return;
	}
	return;
}
void g_chat_history(server_t *server){
	while(server->send==1);
	printf("\n-=| Group Chat History |=-");
	sprintf(server->buffer_out, "8%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	server->recieve=0;
	return;
}
void p_chat_history(server_t *server) {
	char destination[CREDENTIAL_SIZE];
	int valid =0;
	do {
		server->valid_destination = 0;	
		request_users(server);
		printf("PRIVATE CHAT HISTORY BETWEEN YOU AND: ");
		fgets(destination, CREDENTIAL_SIZE, stdin);
		destination[strlen(destination)-1]='\0';
		if (strlen(destination) == 0)
			printf("INPUT CANNOT BE NULL\n");
		else{	
			sprintf(server->buffer_out, "14%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
			server->buffered_out_size=strlen(server->buffer_out)+1;
			server->send=1;

			// wait for the response from the server
			sem_wait(&server->mutex);
			server->recieve=0;
			if(server->valid_destination == 0)
				printf("User does not exist\n");
		}
	} while(!valid && server->valid_destination==0);
	printf("\n-=| Private Chat History with %s |=-", destination);
	sprintf(server->buffer_out, "9%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	server->recieve=0;

	return;
}
int get_destination(char * destination, server_t *server) {
	server->valid_destination = 0;
	do{
		// prompt user for a destination
		fflush(stdin);
		do {
			request_users(server);
			printf("Enter _q to abort\n");
			printf("DESTINATION: ");
			fgets(destination, CREDENTIAL_SIZE, stdin);
			if (strlen(destination) <=1){
				printf("DESTINATION CANNOT BE NULL\n");
			}
			fflush(stdin);
		} while(strlen(destination)<=1);
		destination[strlen(destination)-1]='\0';
		if (strcmp(destination, "_q")==0) {
			return 1;
		}
		// query the server to validate the desination QUERYUSER is a placeholder for a new request mode integer
		sprintf(server->buffer_out,"13%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;

		// wait for the response from the server
		sem_wait(&server->mutex);
		server->recieve= 0;

		// check if the server says the destination is valid - Here i'm just reusing the login successful bit but a new response field may be defined as needed
		if (server->valid_destination==0)
			printf("INVALID TARGET\n");
		// let the listening thread know it is okay to read new messages
		server->recieve=0;

	// if the server responds that the user is still attempting
	// to register, then the username must already be in use.
	}while(!server->valid_destination);
	server->valid_destination = 0;
	return 0;
}
void change_password(server_t *server) {
	char input[CREDENTIAL_SIZE];
	char cur_password[CREDENTIAL_SIZE];
	char password1[CREDENTIAL_SIZE];
	char password2[CREDENTIAL_SIZE];
	do {
		do {
			printf("PLEASE ENTER CURRENT PASSWORD: ");
			fgets(cur_password, CREDENTIAL_SIZE, stdin);
		}while(strlen(cur_password)<=1);
		cur_password[strlen(cur_password)-1]=0;
		do {
			printf("PLEASE ENTER NEW PASSWORD: ");
			fgets(password1, CREDENTIAL_SIZE, stdin);
		}while(strlen(password1)<=1);
	
		do {
			printf("PLEASE REENTER NEW PASSWORD: ");
			fgets(password2, CREDENTIAL_SIZE, stdin);
		}while(strlen(password2)<=1);
		if(strcmp(password1, password2))
			printf("PASSWORDS DO NOT MATCH\n");
		if(strcmp(cur_password, server->password))
			printf("INCORRECT CURRENT CREDENTIAL\n");
		if(strcmp(password1, password2) || strcmp(server->password, cur_password)) {
			printf("ENTER Q TO ABORT, OTHERWISE PRESS ENTER TO CONTINUE\n");
			fgets(input, CREDENTIAL_SIZE, stdin);
			if((strlen(input) == 2) && (input[0] == 'q' || input[0] == 'Q'))
				return;
		}
	}while (strcmp(password1, password2) || strcmp(server->password, cur_password));
	password1[strlen(password1)-1]=0;
	sprintf(server->buffer_out, "4%c%s%c%s%c %c%s", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER, password1);
	server->buffered_out_size = strlen(server->buffer_out)+1;
	//send message to server then wait for response
	server->send=1;
	sem_wait(&server->mutex);
	//printf("%s\n", new_buffer);
		
	//change server->password to password1
	strcpy(server->password, password1);
	printf("PASSWORD SUCCESSFULLY CHANGED\n");

	return;
}
