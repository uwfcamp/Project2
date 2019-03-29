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
		
		if (input[0] == '\n' && server->is_banned_or_kicked == 0) {	//if the enter key was pressed
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
				message[strlen(message)-1]=0;
				fflush(stdin);
				if(strlen(message)<1 && server->is_banned_or_kicked ==0)//reject strings of length 0
					printf("MESSAGE CANNOT BE NULL\n");
			}while(strlen(message)<1 && server->is_banned_or_kicked==0);
			//if message is "_q" then cancel message
			if (strcmp(message, "_q")&& server->is_banned_or_kicked==0) {
				strcat(server->buffer_out, message);//concatenate string message to buffer_out
				server->buffered_out_size=strlen(server->buffer_out)+1;
				server->send=1;
			}
			pthread_mutex_unlock(&server->lock);
			// mutex 1 unlock to replace typing variable
		}

	}while(((strlen(input) != 2) || ((input[0] != 'q') && (input[0] !='Q'))) && server->is_banned_or_kicked==0);
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
	if (server->is_banned_or_kicked ==0) {
		server->in_private_chat=1;
		printf("\n-=|            PRIVATE CHAT             |=-");
		printf("\n-=| HIT ENTER TO SEND A PRIVATE MESSAGE |=-");
		printf("\n-=|     PRESS Q THEN ENTER TO EXIT      |=-\n\n");
		do{
			// read user input
			fgets(input, BUFFER_SIZE, stdin);
			// if input is a null string get user message
			if (input[0]=='\n' && server->is_banned_or_kicked ==0){
				while(server->send==1);
				// 
				dest_valid = get_destination(destination, server);
				if(dest_valid == 0 && server->is_banned_or_kicked ==0) {	//changed from (dest_valid != 1) to prevent -1 from passing
					// mutex semaphore					
					pthread_mutex_lock(&server->lock);// mutex 1 lock to replace typing variable
					//preload header info into server message
					sprintf(server->buffer_out,"6%c%s%c%s%c%s%c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
						char message[BUFFER_SIZE-strlen(server->buffer_out)];
					do {
						printf("Enter _q to abort\nYOUR MESSAGE: ");
						fgets(message, BUFFER_SIZE-strlen(server->buffer_out), stdin);
						if(strlen(message)<=1 && server->is_banned_or_kicked==0){//reject strings of length 0
							printf("MESSAGE CANNOT BE NULL\n");
						}
					}while(strlen(message)<=1 && server->is_banned_or_kicked==0);
					//if message is "_q" then cancel message
					if (strcmp(message, "_q\n") && server->is_banned_or_kicked == 0) {
						strcat(server->buffer_out, message);	//concatenate string message to buffer_out
						server->buffered_out_size=strlen(server->buffer_out)+1;
						pthread_mutex_unlock(&server->lock);
						server->send=1;//set send pending variable
						//mutex semaphore					
						while(server->send==1 && server->is_banned_or_kicked==0);
					}
				}
				// mutex 1 unlock to replace typing variable
			}
	
		}while(((strlen(input)!=2) || (input[0] != 'q' && input[0] !='Q'))&& server->is_banned_or_kicked==0);
		server->in_private_chat=0;
	}
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
			//mutex semaphore		
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
	while (menuChoice != 0 && server->is_banned_or_kicked==0){//keep prompting for selection until quit
		pthread_mutex_lock(&server->lock);
		printf("\n-=| CHAT HISTORY |=-");
		printf("\n1. Group Chat");
		printf("\n2. Private Chat");
		printf("\n0. QUIT");
		printf("\n\nENTER SELECTION: ");
		pthread_mutex_unlock(&server->lock);
		fflush(stdin);//clear input line
		fgets(menuChoice, CREDENTIAL_SIZE, stdin);//read menu choice from stdin
		menuChoice[strlen(menuChoice)-1]='\0';//set last character for null terminated string
		if (atoi(menuChoice)==0 || server->is_banned_or_kicked != 0)
			return;
		else if(atoi(menuChoice) == 1)
			g_chat_history(server);
		else if(atoi(menuChoice) == 2)
			p_chat_history(server);
	}
	return;
}

/*
**************************client side print group chat log function*************************
************************prints group chat history to the screen**************
*/
void g_chat_history(server_t *server){
	while(server->send==1);
	if(server->is_banned_or_kicked==0) {
		printf("\n-=| Group Chat History |=-");
		//compose server message
		sprintf(server->buffer_out, "8%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
	}
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
		show_all_users(server);	//prints all active users to the screen
		printf("PRIVATE CHAT HISTORY BETWEEN YOU AND: ");
		fgets(destination, CREDENTIAL_SIZE, stdin);
		destination[strlen(destination)-1]='\0';	//format as null terminated string
		if (strlen(destination) == 0 && server->is_banned_or_kicked==0)
			printf("INPUT CANNOT BE NULL\n");
		else if(server->is_banned_or_kicked==0){	
			sprintf(server->buffer_out, "14%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
			server->buffered_out_size=strlen(server->buffer_out)+1;
			server->send=1;

			// wait for the response from the server
			sem_wait(&server->mutex);
			if(server->valid_destination == 0 && server->is_banned_or_kicked==0)
				printf("User does not exist\n");
		}
//******* !valid seems unneccesary as it is not modified by the function		
	} while(!valid && server->valid_destination==0 && server->is_banned_or_kicked==0);
	if(server->is_banned_or_kicked==0) {
		printf("\n-=| Private Chat History with %s |=-", destination);
		sprintf(server->buffer_out, "9%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;
		sem_wait(&server->mutex);
	}
	return;
}

/*
**************************client side function to validate a target user*************************
****************************************
*/
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
		} while(strlen(destination)<=1 && server->is_banned_or_kicked==0);
		destination[strlen(destination)-1]='\0';
		if (strcmp(destination, "_q")==0 || server->is_banned_or_kicked!=0) {
			return 1;
		}
		// query the server to validate the desination
		else {
			sprintf(server->buffer_out,"13%c%s%c%s%c%s%c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER);
			server->buffered_out_size=strlen(server->buffer_out)+1;
			server->send=1;
	
			// wait for the response from the server
			sem_wait(&server->mutex);
	
			// check if the server says the destination is valid - Here i'm just reusing the login successful bit but a new response field may be defined as needed
			if (server->valid_destination==0)
				printf("INVALID TARGET\n");
			// let the listening thread know it is okay to read new messages
		}	
		// if the server responds that the user is still attempting to register,
		// then the username must already be in use.
	}while(!server->valid_destination && server->is_banned_or_kicked==0);
	server->valid_destination = 0;
	return 0;
}

/*
**************************client side function to change password*************************
**************************forms a structured request to the server and updates password element**************
*/
void change_password(server_t *server) {
	char input[CREDENTIAL_SIZE];
	char cur_password[CREDENTIAL_SIZE];
	char password1[CREDENTIAL_SIZE];
	char password2[CREDENTIAL_SIZE];
	clear_string(cur_password, CREDENTIAL_SIZE);
	clear_string(input, CREDENTIAL_SIZE);
	clear_string(password1, CREDENTIAL_SIZE);
	clear_string(password2, CREDENTIAL_SIZE);
	//loop until both new passwords match password ma
	do {
		while(strlen(cur_password)<=1 && server->is_banned_or_kicked==0) {
			printf("PLEASE ENTER CURRENT PASSWORD: ");
			fgets(cur_password, CREDENTIAL_SIZE, stdin);
		};
		cur_password[strlen(cur_password)-1]=0;
		while(strlen(password1)<=1 && server->is_banned_or_kicked==0){
			printf("PLEASE ENTER NEW PASSWORD: ");
			fgets(password1, CREDENTIAL_SIZE, stdin);
		}
		while(strlen(password2)<=1 && server->is_banned_or_kicked==0) {
			printf("PLEASE REENTER NEW PASSWORD: ");
			fgets(password2, CREDENTIAL_SIZE, stdin);
		}
		if(strcmp(password1, password2) && server->is_banned_or_kicked==0)
			printf("PASSWORDS DO NOT MATCH\n");
		if(strcmp(cur_password, server->password)&& server->is_banned_or_kicked==0)
			printf("INCORRECT CURRENT CREDENTIAL\n");
		if((strcmp(password1, password2) || strcmp(server->password, cur_password)) || server->is_banned_or_kicked==0) {
			if (server->is_banned_or_kicked==0) {
				printf("ENTER Q TO ABORT, OTHERWISE PRESS ENTER TO CONTINUE\n");
				fgets(input, CREDENTIAL_SIZE, stdin);
			}
			if(((strlen(input) == 2) && (input[0] == 'q' || input[0] == 'Q'))|| server->is_banned_or_kicked!=0)
				return;
		}
	}while ((strcmp(password1, password2) || strcmp(server->password, cur_password))&& server->is_banned_or_kicked==0);
	password1[strlen(password1)-1]=0;
	if(server->is_banned_or_kicked==0) {
		sprintf(server->buffer_out, "4%c%s%c%s%c %c%s", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER, password1);
		server->buffered_out_size = strlen(server->buffer_out)+1;
		//send message to server then wait for response
		server->send=1;
		sem_wait(&server->mutex);
		
		//change server->password to password1
		strcpy(server->password, password1);
	}
	return;
}

void show_all_users(server_t *server) {
	sprintf(server->buffer_out, "17%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out) + 1;
	server->send=1;
	sem_wait(&server->mutex);
	return;
}

/* This function will facilitate the transfer of a file from the
 * client to the server, where it will be stored pending a request
 * for the file.
 *
 * server_t *server
 *	- data structure containing the necessary info to communicate data to the server.
 *
 * return
 *	- N/A
 */
void send_file(server_t *server){
	char destination[CREDENTIAL_SIZE]={0};
	char filename[CREDENTIAL_SIZE];
	int exists = 0;
	int p_exit = -1;
	unsigned long size=0;

	printf("\n-=| SEND A FILE |=-\n\n");

	while(server->send==1);
	// get the filename and destination from the user
	p_exit = get_destination(destination, server);
	if (p_exit != 1)
		exists = get_file_name(filename);
	if (exists)
		size = get_file_size(filename);
	printf("FILE SIZE: %lu\n", size);
	if(size>0) {
		sprintf(server->buffer_out,"10%c%s%c%s%c%s%c%s%c%lu", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, destination, (char)DELIMITER, filename, '_', size);
		server->buffered_out_size=strlen(server->buffer_out)+1;
		server->send=1;
		while(server->send==1);
		FILE *fp = fopen(filename, "rb");
		if (fp != NULL){
			do{
				server->buffered_out_size = fread(server->buffer_out, 1, BUFFER_SIZE, fp);
				size -= server->buffered_out_size;
				server->send=1;
				while(server->send==1);
			}while(size>0);
			fclose(fp);
			printf("FILE SUCCESSFULLY SENT\n");
		}
	}
}



/* This function will ask the user for the name of a file to transfer.
 * if the file named cannot be accessed, 0 is returned. If the user
 * decides to cancel the transfer, 0 is returned. If the file exists,
 * and is accessed successfully, then 1 is returned.
 *
 * char *filename
 *	- A pointer to a string to place user input into
 *
 * return
 *	- Is the filename given valid? 1=yes, 0=no
 */
int get_file_name(char *filename){
	// prompt user for a filename
	fflush(stdin);
	do {
		printf("Enter _q to abort\n");
		printf("FILENAME: ");
		fgets(filename, CREDENTIAL_SIZE, stdin);
		if (strlen(filename) <=1){
			printf("FILENAME CANNOT BE NULL\n");
		}
		fflush(stdin);
	} while(strlen(filename)<=1);
	filename[strlen(filename)-1]='\0';

	// check if user wants to cancel
	if (strcmp(filename, "_q")==0) {
		return 0;
	}

	// try to open the file
	FILE *fp = fopen(filename, "r");
	if (fp==NULL)
		return 0;
	fclose(fp);

	// file was accessed successfully
	return 1;
}



/* This function will retrieve the size of a given file from the file
 * description tables in the system. If there is a problem getting the
 * size of the file, 0 is returned.
 *
 * char *filename
 *	- A pointer to a string containing the file's name
 *
 * return
 *	- An unsigned long, denoting the size of the file
 */
unsigned long get_file_size(char *filename){
	struct stat st;
	if (stat(filename, &st) == 0)
		return st.st_size;
	return 0;
}



/* This function will print a file transfer menu, get the
 * user's selection, and either send a file to the server,
 * retrieve a file from the server, or return to the main
 * menu based on the user input.
 *
 * server_t *server
 *	- A pointer to the structure containing information about the server
 *
 * return
 *	- N/A
 */
void file_menu(server_t *server){
	char input[CREDENTIAL_SIZE];
	int menuChoice=1;
	int valid=0;
	int i=0;

	// keep looping this menu until the user quits
	while (menuChoice != 0){
		printf("\n-=| FILE TRANSFER |=-");
		printf("\n1. Send a file");
		printf("\n2. Recieve a file");
		printf("\n0. QUIT");
		printf("\n\nEnter an action: ");

		// get input, and verify it is valid
		do{
			valid=1;
			fgets(input, CREDENTIAL_SIZE, stdin);
			input[strlen(input)-1] = '\0';
			for (i=0; i<strlen(input); i++){
				if (input[i]<'0' || input[i]>'9'){
					valid=0;
					break;
				}
			}
			if (valid)
				menuChoice = atoi(input);
		}while(!valid);

		// perfom function based on user input
		if(menuChoice == 1)
			send_file(server);
		else if(menuChoice == 2)
			retrieve_file(server);
		else if(menuChoice == 0)
			return;
	}
	return;
}



/* This function will request a list of files intended for the user from the
 * server, ask the user which one they would like, and then request the file
 * from the server. The program will then wait for the file to be fully transferred.
 *
 * server_t *server
 *	- A pointer to the structure containing information about the server
 *
 * return
 *	- N/A
 */
void retrieve_file(server_t *server){
	char input[CREDENTIAL_SIZE];

	// request a list of the files meant for the user
	sprintf(server->buffer_out,"18%c%s%c%s%c %c ", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
	server->buffered_out_size = strlen(server->buffer_out) + 1;
	server->send=1;
	sem_wait(&server->mutex);

	//get the user's input
	printf("WHICH FILE WOULD YOU LIKE TO RETRIEVE: ");
	fgets(input, CREDENTIAL_SIZE, stdin);
	input[strlen(input)-1] = '\0';

	// request specific file from server
	sprintf(server->buffer_out,"19%c%s%c%s%c %c%s", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER, input);
	server->buffered_out_size = strlen(server->buffer_out) + 1;
	server->send=1;
	sem_wait(&server->mutex);
}
