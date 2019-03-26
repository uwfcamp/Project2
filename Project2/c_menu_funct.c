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
			pthread_mutex_lock(&server->lock);
			//read user input untill the message exists and return has been hit
			do {
				printf("Enter _q to abort\n");
				printf("YOUR MESSAGE: ");
				fgets(message, BUFFER_SIZE-strlen(server->buffer_out), stdin);
				fflush(stdin);
				if(strlen(message)<=1)//reject strings of length 0
					printf("MESSAGE CANNOT BE NULL\n");
			}while(strlen(message)<=1);
			//if message is "_q" then cancel message
			if (strcmp(input, "_q\n")) {
				strcat(server->buffer_out, message);//concatenate string message to buffer_out
				server->buffered_out_size=strlen(server->buffer_out)+1;
				server->send=1;
			}
			pthread_mutex_unlock(&server->lock);
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
//***************************Exchange for mutex semaphore!!!!!!					
				pthread_mutex_lock(&server->lock);// mutex 1 lock to replace typing variable
				//preload header info into server message
				sprintf(server->buffer_out,"6%c%s%c%s%c%s%c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
					char message[BUFFER_SIZE-strlen(server->buffer_out)];
				do {
					printf("Enter _q to abort\nYOUR MESSAGE: ");
					fgets(message, BUFFER_SIZE-strlen(server->buffer_out), stdin);
					if(strlen(message)<=1){//reject strings of length 0
						printf("MESSAGE CANNOT BE NULL\n");
					}
				}while(strlen(message)<=1);
				//if message is "_q" then cancel message
				if (strcmp(message, "_q\n")) {
					strcat(server->buffer_out, message);	//concatenate string message to buffer_out
					server->buffered_out_size=strlen(server->buffer_out)+1;
					pthread_mutex_unlock(&server->lock);
					server->send=1;//set send pending variable
//***************************Exchange for mutex semaphore!!!!!!					
					while(server->send==1);
				}
			}
			// mutex 1 unlock to replace typing variable
		}

	}while((strlen(input)!=2) || (input[0] != 'q' && input[0] !='Q'));
	server->in_private_chat=0;
}

/*
**************************client side logout function*************************
**************marks user inactive in user file, returns UI to login screen
*/
void logout(server_t *server) {
	//compose logout server message
	sprintf(server->buffer_out, "3%c%s%c%s%c %c " , (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	printf("logging out\n");
	server->buffered_out_size=strlen(server->buffer_out)+1;
//***************************Exchange for mutex semaphore!!!!!!		
	server->send=1;	//set send pending variable
	pthread_mutex_unlock(&server->lock);
	return;
}


/*
**************************client side request user list function*************************
*************************prints all online users to the screen***********************
*/
void request_users(server_t *server){
	while(server->send==1);
	//compose server message
	sprintf(server->buffer_out, "5%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
}

/*
**************************client side request chat log function*************************
****************selects either group or private chat history**************
*/
void chat_history(server_t *server) {
	char menuChoice[CREDENTIAL_SIZE];
	while (menuChoice != 0){//keep prompting for selection until quit
		printf("\n-=| CHAT HISTORY |=-");
		printf("\n1. Group Chat");
		printf("\n2. Private Chat");
		printf("\n0. QUIT");
		printf("\n\nENTER SELECTION: ");
		fflush(stdin);//clear input line
		fgets(menuChoice, CREDENTIAL_SIZE, stdin);//read menu choice from stdin
		menuChoice[strlen(menuChoice)-1]='\0';//set last character for null terminated string
		if(atoi(menuChoice) == 1)
			g_chat_history(server);
		else if(atoi(menuChoice) == 2)
			p_chat_history(server);
		else if(atoi(menuChoice) == 0)
			return;
	}
	return;
}

/*
**************************client side print group chat log function*************************
************************prints group chat history to the screen**************
*/
void g_chat_history(server_t *server){
	while(server->send==1);
	printf("\n-=| Group Chat History |=-");
	//compose server message
	sprintf(server->buffer_out, "8%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);
	return;
}

/*
**************************client side print private chat log function*************************
**************************prints private chat history to the screen**************
*/
void p_chat_history(server_t *server) {
	char destination[CREDENTIAL_SIZE];
//**int valid is apparently redundent and unused	
	int valid =0;
	//loop the prompt until a valid target is selected
	do {
		server->valid_destination = 0;	//set validity to default to false
//******replace with function to print all registered users********		
		request_users(server);	//prints all active users to the screen
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
			if(server->valid_destination == 0)
				printf("User does not exist\n");
		}
//******* !valid seems unneccesary as it is not modified by the function		
	} while(!valid && server->valid_destination==0);
	printf("\n-=| Private Chat History with %s |=-", destination);
	sprintf(server->buffer_out, "9%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
	server->buffered_out_size=strlen(server->buffer_out)+1;
	server->send=1;
	sem_wait(&server->mutex);

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

		// check if the server says the destination is valid - Here i'm just reusing the login successful bit but a new response field may be defined as needed
		if (server->valid_destination==0)
			printf("INVALID TARGET\n");
		// let the listening thread know it is okay to read new messages

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
		
	//change server->password to password1
	strcpy(server->password, password1);

	return;
}
