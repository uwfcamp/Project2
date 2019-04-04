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

int get_menu_option(char * userInput) {
	char *  search = "\n ";
	char * token;
	int i;
	token = strtok(userInput, search);
	for(i=0;i<strlen(token);i++) {
		token[i]=tolower(token[i]); 
	if(strcmp(token ,"quit")==0)
		return 0;
	if(strcmp(token, "help")==1)
		return 1;
	else if (strcmp(token, "count")==0){
		if((token = strtok(NULL, search)) != NULL) {
			if(strcmp(token, "current")==0)
				return 2;
			else if(strcmp(token, "all")==0)
				return 3;
			else // invalid command after count
				return -2;
		}
		else //invalid syntax for count
			return -2;
	}
}

void * menuThread(void * param) {
	char userInput[INPUT_SIZE];
	int menuOption = -1;
	print_menu();
	do {
		do{
			printf(">> ");
			fgets(userInput, INPUT_SIZE, stdin);
		} while(strlen(userInput) <= 1);
		menuOption=get_menu_option(userInput);
		switch(menuOption) {
			case 0:	//quit case
				//end all connections
				//printf("Quiting application\n"); // delete late
				break;
			case 1: //help case
				print_menu();
				break;
			case 2: // count current
				break;
			case 3: // count all
				break;
			case -2: // invalid count syntax 
				printf("Invalid input for count, try 'count all' or 'count current'\n");
				break;
			default: // error case
				printf("INVALID INPUT\n");
				break;	
		}
		
	}while(menuOption != 0);
	pthread_exit(NULL);
}

void * clientThread(void * param) {
	
	pthread_exit(NULL);
}
