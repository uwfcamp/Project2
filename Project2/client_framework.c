// Client side C/C++ program to demonstrate Socket programming

#include "Definitions.h"
#include "parse.h"
#include "c_menu_funct.h"
#include "c_login_funct.h"
#define IP_ADDRESS "127.0.0.1"







int menu_input(void);
int main_menu(server_t *server);

server_t *build_server_structure(void);
void disconnect(server_t *server);
void *server_communication(void *vargp);


int main(int argc, char const *argv[])
{
    struct sockaddr_in serv_addr;
    server_t *server = build_server_structure();
    int quit = -1;

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

    while(server->connected && quit != 0){
	if(!server->logged_in)
		quit = login_menu(server);
	else
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
			server->buffered_out_size=0;
			server->send=0;
		}

		// recieve message from server
		if (server->recieve==0){
			server->buffered_in_size = recv(server->socket, server->buffer_in, server->buffer_size, MSG_NOSIGNAL | MSG_DONTWAIT);

			int err = errno;

			//check if any data was actually recieved
			if (server->buffered_in_size>0) {
				server->recieve=1;
			}
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
			/*
********************MY EDITS*******************************************
*****************selective rendering of group and private messages*****
*/
			// print out chat messages
			if ((mode==6) && (server->in_private_chat==1 && destination[0]!=' ')){
				printf("%s: %s", username, body);
			}
			else if ((mode==7) && (server->in_group_chat==1 && destination[0]==' ')){
				printf("%s: %s", username, body);
			}
//*************************************************************			
			else if (mode==5||mode==8||mode==9){
				printf("\n%s\n", body);
			}
			else if (mode==13 || mode == 14) {
				if (strcmp(body, "Y")==0){
					server->valid_destination=1;
				}
				else {
					server->valid_destination=0;
				}
			}

			//mutex 1 unlock to replace typing variable

			// once the recieved message has been utalyzed,
			// the buffer must be cleared, except in instances
			// where the main thread must handle the response.
			
			if(mode !=5 && mode != 13 && mode != 8 && mode != 9 && mode != 14){
				clear_string(server->buffer_in, BUFFER_SIZE);
				server->buffered_in_size=0;
				server->recieve=0;
			}
			else{
				server->buffered_in_size=0;
				clear_string(server->buffer_in, BUFFER_SIZE);
				server->recieve=2;
			}
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
	fflush(stdout);
	switch(selection){
		case 0:
			break;
		case 1:
			request_users(server);
			break;
		case 2:
			group_chat(server);
			break;
		case 3:
			private_chat(server);
			break;
		case 4: //view chat history
			chat_history(server);
			break;
		case 5: 
			break;
		case 6:
			break;
		case 7:
			logout(server);
			server->logged_in=0;
			server->username[0]='\0';
			server->password[0]='\0';
			break;
		case 8:
			break;
	}

	return selection;
}
