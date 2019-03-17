// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Definitions.h"
#include "parse.h"

#define IP_ADDRESS "127.0.0.1"



typedef struct server_s{
	int socket;		// identifier for the server socket
	char username[CREDENTIAL_SIZE];	// store the username of the user
	char password[CREDENTIAL_SIZE];	// store the password of the user
	char *buffer_in;	// Pointer to the buffer of stuff coming in from the server
	char *buffer_out;	// Pointer to the buffer of stuff going out to the server
	int buffer_size;	// The max size of buffer
	int buffered_in_size;	// The number of bytes in the input buffer
	int buffered_out_size;	// The number of bytes in the output buffer
	int send;		// 0 if there is nothing to be sent, 1 if there is
	int recieve;		// 0 if nothing has been recieved, 1 if something has
	int connected;		// 0 if not connected, 1 if connected
	int logged_in;		// 0 if not logged in, 1 if logged in
	int typing;
	int in_group_chat;
	int in_private_chat;
	char username_private_chat[CREDENTIAL_SIZE];
}server_t;



int login_menu(server_t *server);
int menu_input(void);
void registration_input(server_t *server);
void login_input(server_t *server);
int main_menu(server_t *server);
void group_chat(server_t *server);



server_t *build_server_structure(void);
void disconnect(server_t *server);
void *server_communication(void *vargp);



int main(int argc, char const *argv[])
{
    struct sockaddr_in serv_addr;
    server_t *server = build_server_structure();


    if ((server->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr,"ERROR: socket creation error\n");
        server->connected=0;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr)<=0)
    {
        fprintf(stderr,"ERROR: invalid address and/or address not supported\n");
        server->connected=0;
    }

    if (connect(server->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "ERROR: connection failed\n");
        server->connected=0;
    }

    pthread_t tid;

    if (pthread_create(&tid, NULL, server_communication, (void *)server)){
	fprintf(stderr,"ERROR: could not create thread\n");
	server->connected=0;
    }

    while(server->connected){
	if(!server->logged_in)
		login_menu(server);

	if(server->logged_in)
		main_menu(server);
    }

    pthread_join(tid, NULL);
    disconnect(server);

    return 0;
}



// This function will check for a message that
// needs to be sent and a message that is
// being recieved.
void *server_communication(void *vargp){
	// send message to server
	server_t *server = (server_t *)vargp;

	while(server->connected){

		//send message to server
		if (server->send==1){
			if (send(server->socket , server->buffer_out, server->buffered_out_size , MSG_NOSIGNAL | MSG_DONTWAIT)<0)
				server->connected=0;
			clear_string(server->buffer_out, BUFFER_SIZE);
			server->buffer_out[0]='\0';
			server->buffered_out_size=0;
			server->send=0;
		}

		// recieve message from server
		if (server->recieve==0){
			server->buffered_in_size = recv(server->socket, server->buffer_in, server->buffer_size, MSG_NOSIGNAL | MSG_DONTWAIT);

			int err = errno;

			//check if any data was actually recieved
			if (server->buffered_in_size>0)
				server->recieve=1;
			else if (err == EAGAIN || err == EWOULDBLOCK)
				server->buffered_in_size=0;
			else if (server->buffered_in_size<0)
				server->connected=0;
		}
		// need to create function to handle
		// the different types of messages
		if (server->recieve==1 && server->typing==0 && server->logged_in==1){
			//mutex 1 lock to replace typing variable
			int mode;
			char body[BUFFER_SIZE];
			char username[CREDENTIAL_SIZE];
			char password[CREDENTIAL_SIZE];
			char destination[CREDENTIAL_SIZE];

			parse_message(server->buffer_in, &mode, username, password, destination, body);


			// print out chat messages
			if (server->buffer_in[0]=='7' && (server->in_group_chat==1 || server->in_private_chat==1)){
				printf("%s: %s", username, body);
			}

			//mutex 1 unlock to replace typing variable

			// once the recieved message has been utalyzed, the buffer must be cleared
			server->buffer_in[0]='\0';
			server->buffered_in_size=0;
			server->recieve=0;
		}
	}
	return NULL;
}



// This function builds the stucture used for
// communication to and from the server
server_t *build_server_structure(void){
	server_t *server = (server_t *)malloc(sizeof(server_t));
	server->username[0]='\0';
	server->password[0]='\0';
	server->socket = 0;
	server->buffer_size=BUFFER_SIZE;
	server->buffer_in=(char *)malloc(BUFFER_SIZE);
	server->buffer_in[0]='\0';
	server->buffered_in_size=0;
	server->buffer_out=(char *)malloc(BUFFER_SIZE);
	server->buffer_out[0]='\0';
	server->buffered_out_size=0;
	server->send=0;
	server->recieve=0;
	server->connected=1;
	server->logged_in=0;
	server->typing=0;
	server->in_group_chat=0;
	server->in_private_chat=0;
	server->username_private_chat[0]='\0';

	return server;
}



// This function disconnects from the server
// and frees up the memory allocated for
// communication. Should be called after
// communication is done.
void disconnect(server_t *server){
	close(server->socket);
	free(server->buffer_in);
	free(server->buffer_out);
	free(server);
}



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



int menu_input(void){
	char input[CREDENTIAL_SIZE]={0};
	int valid = 0, i;
	do{
		printf("Enter an action: ");
		scanf(" %s", input);
		while(getchar()!='\n');
		for(i=0; input[i]!='\0'; i++){
			valid = 0;
			if (input[i]<'0' || input[i]>'9')
				break;
			valid = 1;
		}
	}while(!valid);
	return atoi(input);
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



int main_menu(server_t *server){
	int selection = -1;

	// print the menu
	printf("\n-=| MAIN MENU |=-\n\n");
	printf("1. View current online number\n");
	printf("2. Enter the group chat\n");
	printf("3. Enter the private chat\n");
	printf("4. View chat history\n");
	printf("5. File transfer\n");
	printf("6. Change the password\n");
	printf("7. Logout\n");
	printf("8. Administrator\n");
	printf("0. Return to login screen\n");
	printf("\n");

	// get the selection
	do{
		selection = menu_input();
	}while(selection<0 || selection>8);

	// perform selection
	switch(selection){
		case 0:
			break;
		case 1:
			break;
		case 2: // this is the only feature we are initially worried about implementing
			group_chat(server);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			server->logged_in=0;
			server->username[0]='\0';
			server->password[0]='\0';
			break;
		case 8:
			break;
	}

	return selection;
}



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
