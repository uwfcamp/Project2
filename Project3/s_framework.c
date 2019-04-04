//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.c

#include "Definitions.h"
#include "s_framework.h"
int main(int argc, char **argv) {
	server_t *clientList = (server_t *) malloc(sizeof(server_t));
	clientList->connected=0;
	clientList->socket=0;
	clientList->last=NULL;
	clientList->next=NULL;
	clientList->ip[0]='\0';
	char input[INPUT_SIZE];

	//option to let user set up IP or use local network as default
	do {
		printf("PRESS ENTER TO USE LOCAL NETWORK AS DEFAULT OR ENTER IP THAT YOU WANT TO USE\n>> ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input)-1]=0;
		if(strlen(input)==0)//user just pressed enter
			strcpy(clientList->ip, DEFAULT_IP);
		else if (strlen(input)<16 && strlen(input)>6)//valid IP case
			strcpy(clientList->ip, input);
	}while(clientList->ip[0]=='\0');
	//startup

	//When a client connects, a new thread is created to handle each new connection.

	//shutdown

	return 0;
}
void print_menu(void) {
	printf("-=| Server Menu |=-\n");
	printf("help: Display all commands for ther server.\n");
	printf("quit: Quit the server.\n");
	printf("count current: Display number of currently active users.\n");
	printf("count all: Display the total number of system visitors.\n");
	return;
}
