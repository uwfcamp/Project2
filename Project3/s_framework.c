//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.c



// headers
#include "Definitions.h"
#include "s_framework.h"



// global control variables
int force_logoff=0;
unsigned long clientCountAll=0;
unsigned long clientCountCurrent=0;



int main(int argc, char **argv) {
	int serverSocket;
	int newSocket;
	char ip[IP_LENGTH];
	ip[0]='\0';

	char input[INPUT_SIZE];
	server_t *clientList = NULL; // the root of the client list
	server_t **rootPointer;
	rootPointer = (server_t **)malloc(sizeof(server_t *));
	*rootPointer = clientList; // pointer to the root pointer
	pthread_t menuTID; // the thread id for the menu thread

	//option to let user set up IP or use local network as default
	do {
		printf("PRESS ENTER TO USE LOCAL NETWORK AS DEFAULT OR ENTER IP THAT YOU WANT TO USE\n>> ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input)-1]=0;
		if(strlen(input)==0)//user just pressed enter
			strcpy(ip, DEFAULT_IP);
		else if (strlen(input)<16 && strlen(input)>6)//valid IP case
			strcpy(ip, input);
	}while(ip[0]=='\0');

        // create the socket for listening for incoming traffic
        printf("STATUS: creating socket\n");
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0))<0){
                //if the socket could not be created, end the process
                fprintf(stderr, "ERROR: could not create socket\n");
                return 0;
        }

	// free the socket up
	int on=1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

        // create the address structure for the listening socket
        printf("STATUS: creating address\n");
        struct sockaddr_in socket_address;
        socket_address.sin_family = AF_INET;
        socket_address.sin_port = htons(PORT);
        socket_address.sin_addr.s_addr = inet_addr(ip);
        int address_length = sizeof(socket_address);

        // bind the socket to the address
        printf("STATUS: binding socket to address\n");
        if (bind(serverSocket,(struct sockaddr *)&socket_address,sizeof(socket_address))<0){
                //if the socket cannot be bound to the address, end the process
                fprintf(stderr, "ERROR: could not bind socket to address\n");
                return 0;
        }

        // set the process to listen on the socket for incoming data
        printf("STATUS: setting process to listen to socket\n");
        if (listen(serverSocket,50)<0){
                // if the process is unable to listen to the socket, end the process
                fprintf(stderr, "ERROR: could not listen to socket\n");
                return 0;
        }

	// startup the menu thread
	pthread_create(&menuTID, NULL, menuThread, (void *)rootPointer);

	// set the server's socket to not block
	int flags = fcntl(serverSocket, F_GETFL);
	fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

	//while force_logoff != 1 When a client connects, a new thread is created to handle each new connection.
	while(force_logoff != 1) {
                //listen on the port for new connection requests
		newSocket=accept(serverSocket,(struct sockaddr*)&socket_address,(socklen_t *)&address_length);

		// if a connection was created, establish it as a client
		if (newSocket>0){
			clientList = clientCreate(clientList, newSocket);
			clientCountAll++;
			clientCountCurrent++;
		}

		// loop through connections, and find ones that need to be removed
		server_t *client = clientList;
		while (client!=NULL){
			if (!(client->connected)){
				server_t *temp = client->next;
				clientList = clientTerminate(clientList, client->socket);
				clientCountCurrent--;
				client = temp;
			}
			else{
				client=client->next;
			}
		}
	}

	// call clientTerminate for all clients
	while(clientList!=NULL){
		clientList = clientTerminate(clientList, clientList->socket);
		clientCountCurrent--;
	}

	//shutdown
	pthread_join(menuTID, NULL);
	free(rootPointer);

	return 0;
}



void print_menu(void) {
	printf("-=| Server Menu |=-\nhelp: Display all commands for ther server.\nquit: Quit the server.\ncount current: Display number of currently active users.\ncount all: Display the total number of system visitors.\n");
	return;
}



int get_menu_option(char * userInput) {
	char *  search = "\n ";
	char * token;
	int i;
	for(i=0;i<strlen(userInput);i++)
		userInput[i]=tolower(userInput[i]);
	token = strtok(userInput, search);
	if(strcmp(token ,"quit")==0){
		force_logoff=1;
		return 0;
	}
	if(strcmp(token, "help")==0)
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
	return -1;
}



void * menuThread(void * param) {
	server_t **firstClient = (server_t **)param;
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
				force_logoff=1;
				//printf("Quiting application\n"); // delete late
				break;
			case 1: //help case
				print_menu();
				break;
			case 2: // count current
				printf("%lu\n", clientCountCurrent);
				break;
			case 3: // count all
				printf("%lu\n", clientCountAll);
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



/* This thread function will contain all the handling
 * of communication between a thread and server.
 *
 * void *param
 *	- cast to server_t *
 *      - pointer to a data structure storing information about the listening socket
 */
void * clientThread(void * param){
	server_t *client = (server_t *)param;

	// loop while the connection is valid
	// and process the data recieved
	while(!(client->force_close) && (client->connected)){
		int error = 0, recieved=0;
		// errno, declared in errno.h, should start from a known state
		// this value is thread safe however, and a change in one thread
		// will not affect the errno in another thread.
		errno=0;

		char buffer_in[BUFFER_SIZE];
		buffer_in[0]=0;//set first element to null
		// recieve the command from the client and respond accordingly
		if((recieved=recv(client->socket, buffer_in, BUFFER_SIZE, MSG_NOSIGNAL | MSG_DONTWAIT))>0){
			char *command = strtok(buffer_in, "\n ");
			char *param = strtok(NULL, "\n");
			if (strcmp(command, "pwd")==0){
				char buffer[BUFFER_SIZE];
				buffer[0]=0;
				getcwd(buffer, BUFFER_SIZE);
				printf("\n%s\n>> ", buffer);
				fflush(stdout);
				send(client->socket, buffer, strlen(buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
			}
			else if (strcmp(command, "ls")==0){
				char buffer[BUFFER_SIZE];
				buffer[0]=0;
				DIR *d = opendir("./");
				struct dirent *dir;
				if (d != NULL){
					strcpy(buffer,"\n");
					while((dir=readdir(d))!=NULL){
						strcat(buffer, dir->d_name);
						strcat(buffer, "\n");
					}
					closedir(d);
				}
				send(client->socket, buffer, strlen(buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
			}
			else if (strcmp(command, "echo")==0){
				send(client->socket, param, strlen(param), MSG_NOSIGNAL | MSG_DONTWAIT);
			}
			int i;
			for(i=0;i<BUFFER_SIZE;i++)
				buffer_in[i]=0;

		}

		// this checks if the connection has been broken
		if (error == EPIPE || recieved == 0){
			client->connected = 0;
		}
	}

	// terminate the thread
	pthread_exit(NULL);
}



/* This function will remove a client from the connection
 * list and set the thread to terminate.
 *
 * server_t *client
 *	- pointer to the root of the client list
 * int socket
 *	- integer identifying the socket to disconnect
 *
 * return server_t *
 *	- pointer to the new root of the client list
 */
server_t *clientTerminate(server_t *client, int socket){
	server_t *root = client;

	// loop through all the clients until the desired one is located
	if (client != NULL){
		while(client->socket != socket){
			client = client->next;
			if (client==NULL)
				break;
		}
	}

	// if the client was located, terminate it
	if (client!=NULL){
		client->force_close=1;
		pthread_join(client->TID, NULL);

		// remove the element from the link list
		if (client->last != NULL && client->next != NULL){ // client is in the middle of the list
			client->last->next = client->next;
			client->next->last = client->last;
		}
		else if (client->last != NULL){ // client is at the end of the list
			client->last->next = NULL;
		}
		else if (client->next != NULL){ // client is at the beginning of the list
			root = client->next;
			client->next->last = NULL;
		}
		else{ // client is the only element in the list
			root = NULL;
		}

		// de-allocate memory for the client
		close(client->socket);
		free(client);
	}

	// return the root of the client list
	return root;
}



/* This function will add a client to the list of clients.
 *
 * server_t *client
 *	- pointer to the root of the client list
 * int socket
 *	- integer identifying the socket that the new client is connected to
 *
 * return server_t *
 *	- pointer to the new root of the client list
 */
server_t *clientCreate(server_t *clientList, int socket){
	server_t *root = clientList;
	server_t *list = clientList;

	// create new client structure
	server_t *newClient = (server_t *)malloc(sizeof(server_t));
	newClient->force_close = 0;
	newClient->connected = 1;
	newClient->socket = socket;

	// add the client to the list
	if (root == NULL){ // client list is empty
		newClient->last=NULL;
		newClient->next=NULL;
		root = newClient;
	}
	else{ // client list has at least one element
		while(list->next != NULL)
			list = list->next;
		list->next = newClient;
		newClient->last = list;
		newClient->next = NULL;
	}

	// create the thread for the client
	pthread_create(&(newClient->TID), NULL, clientThread, (void *)newClient);

	// return the root of the list
	return root;
}
