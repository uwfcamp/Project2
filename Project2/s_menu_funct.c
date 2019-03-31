//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_menu_funct.c

#include "Definitions.h"
#include "parse.h"
#include "s_menu_funct.h"



/* This function will send a message to all online users
 *
 * client_list_t *clientList
 *	- link list of all the clients
 * int sender_socket
 *	- the socket of the client sending the message
 * char *message
 *	- string containing the message to be sent
 * char *sender
 *	- string containing the username of the client sending the message
 *
 * return
 *	- N/A
 */
void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender){
    // create the formatted message to be sent
    char new_buffer[BUFFER_SIZE];
    sprintf(new_buffer, "7%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, " ", (char)DELIMITER, message);

    // loop through all the nodes of the link list
    while(clientList != NULL){
	// if the client is connected and logged in
        if (clientList->socket != sender_socket && clientList->connected==1 && clientList->logged_in==1){
		// send message
	        send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
        clientList=clientList->next;
    }
    printf("STATUS: message sent to all users\n");
}



/* This function will send a message to a specific user, and
 * only that user.
 *
 * client_list_t *clientList
 *	- link list of all the clients
 * char *message
 *	- string containing the message to be sent
 * char *destination
 *	- string containing the username of the client the message is for
 * char *sender
 *	- string containing the username of the client sending the message
 *
 * return
 *	- N/A
 */
void private_message(client_list_t *clientList, char *message, char *destination, char *sender){
    int i=0;
    char new_buffer[BUFFER_SIZE];
	
    // log the message into the private message log
    log_into_private(sender, destination, message);
	
    // create formatted message for sending to clients
    sprintf(new_buffer, "6%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, destination, (char)DELIMITER, message);

    // loop through all clients in link list
    while(clientList != NULL){
	// check if the client matches the target user
        if (strcmp(destination, clientList->username)==0){
		// if the client is online and logged in
		if (clientList->connected==1 && clientList->logged_in==1){
			// send the message to the client
	        	send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
                	printf("STATUS: message sent to %s\n", destination);
			i++;
		}
	}
        clientList=clientList->next;
    }

    // if the user was not online to recieve the message, print the message to the console
    if(i==0)
    	printf("STATUS: message could not be sent to %s\n", destination);
}



/* This function will log a message into the group chat log file.
 * It will log the time of the message, who sent the message, and
 * the message itself.
 *
 * char *username
 *	- string containing the username of the client sending the message
 * char *body
 *	- the message itself
 *
 * return
 *	- N/A
 */
void log_into_group(char *username, char *body){
	// open group chat log file
	FILE *fp;
	fp=fopen("groupchat.txt", "a");

	// figure out current time, and create string of the time
	time_t rawtime;
	time(&rawtime);
	struct tm *info = localtime(&rawtime);
	char timestamp[MAX_TIME_SIZE];
	clear_string(timestamp,MAX_TIME_SIZE);
	strcpy(timestamp,asctime(info));
	timestamp[strlen(timestamp)-1]='\0'; // removing newline

	// print the timestamp, sending client, and the message to the group chat log file
	fprintf(fp, "%s - %s: %s",timestamp, username, body);

	fclose(fp);
	return;
}



/* This function will log a message into the private chat log file.
 * It will log the time of the message, who sent the message, who
 * the message is for, and the message itself.
 *
 * char *sender
 *	- string containing the username of the client sending the message
 * char *reciever
 *	- string containing the username of the client the message is meant for
 * char *body
 *	- the message itself
 *
 * return
 *	- N/A
 */
void log_into_private(char *sender, char *reciever, char *body){
	// open private chat log file
	FILE *fp;
	fp=fopen("privatechat.txt","a");

	// figure out current time, and create string of the time
	time_t rawtime;
	time(&rawtime);
	struct tm *info = localtime(&rawtime);
	char timestamp[MAX_TIME_SIZE];
	clear_string(timestamp,MAX_TIME_SIZE);
	strcpy(timestamp,asctime(info));
	timestamp[strlen(timestamp)-1]='\0'; // removing newline

	// print the timestamp, sending client, recieving client, and the message to the private chat log file
	fprintf(fp, "%s%c%s%c%s%c%s", timestamp, (char)DELIMITER, sender, (char)DELIMITER, reciever, (char)DELIMITER, body);

	fclose(fp);
	return;
}



/* This function will send a message to the client, with the body containing
 * a list of all online users, and mode 5.
 *
 * char *username
 *	- the current client's username
 * char *password
 *	- the current client's password
 * client_list_t *clientList
 *	- link list of all clients
 * client_list_t *current
 *	- the current client checking for online users
 *
 * return
 *	- N/A
 */
void show_users(char * username, char * password, client_list_t *clientList, client_list_t *current){
	char new_buffer[BUFFER_SIZE];
	int i=0;
	char temp[CREDENTIAL_SIZE];
	client_list_t *list = clientList;
	sprintf(new_buffer, "5%c %c %c %cCURRENT ONLINE USER LIST: ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(list != NULL) {
		if(list->logged_in == 1){
			strcat(new_buffer, list->username);
			strcat(new_buffer, " ");
			i++;
		}
		list = list->next;
	}
	strcat(new_buffer, "\nCURRENT NUMBER OF ONLINE USERS: ");
	sprintf(temp, "%d\n", i);
	strcat(new_buffer, temp);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;	
}



/* This function will send a message to the client, with the body containing
 * all group chat messages.
 *
 * client_list_t *current
 *	- the current client trying to check for group chat messages
 *
 * return
 *	- N/A
 */
void send_group_log(client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
	sprintf(new_buffer, "8%c %c %c %c\n", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	FILE * fp;
	fp = fopen("groupchat.txt", "r");
	while(fgets(temp, BUFFER_SIZE, fp)!=NULL)
		strcat(new_buffer, temp);	
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	fclose(fp);
}



/* This function will send a message to the client, with the body containing
 * all messages between the client and another specified user.
 *
 * char *destination
 *	- the username that the client is checking for private messages with
 * client_list_t *current
 *	- the current client trying to check for private messages
 *
 * return
 *	- N/A
 */
void send_private_log(char * destination, client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
	char log[BUFFER_SIZE];
	clear_string(log, BUFFER_SIZE);
	sprintf(new_buffer, "9%c %c %c %c\n", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	FILE * fp;
	char * token;
	char search[1];
	search[0] = (char)DELIMITER;
	search[1] = '\0';
	char u_to[CREDENTIAL_SIZE];
	char u_from[CREDENTIAL_SIZE];
	char u_timestamp[CREDENTIAL_SIZE];
	char message[BUFFER_SIZE-CREDENTIAL_SIZE*3];
	fp = fopen("privatechat.txt", "r");
	while(fgets(temp, BUFFER_SIZE, fp)!=NULL) {
		clear_string(u_timestamp, CREDENTIAL_SIZE);
		clear_string(u_to, CREDENTIAL_SIZE);
		clear_string(u_from, CREDENTIAL_SIZE);
		clear_string(message, BUFFER_SIZE-CREDENTIAL_SIZE*3);
		token = strtok(temp, search);
		strcpy(u_timestamp, token);
		token = strtok(NULL, search);
		strcpy(u_from, token);
		token = strtok(NULL, search);
		strcpy(u_to, token);
		token = strtok(NULL, search);	
		strcpy(message, token);
		if(((strcmp(u_from, current->username) == 0) && (strcmp(u_to, destination))==0) || ((strcmp(u_to, current->username)==0) && strcmp(u_from, destination)==0)) {
			sprintf(temp, "%s - %s: %s", u_timestamp, u_from, message);
			strcat(log, temp);
		}
		clear_string(temp, BUFFER_SIZE);
	}
	if(strlen(log)==0)
		sprintf(log, "Sorry, there is no chat history between you and %s\n", destination);
	strcat(new_buffer, log);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	fclose(fp);
	return;
}



/* This function will confirm to the current client whether another user is online,
 * by sending a mode 13 message, with a character in the body of the message.
 * Y = yes, the user is online
 * N = no, the user is not online
 *
 * char *destination
 *	- the username that the client is checking online status about
 * client_list_t *clientList
 *	- link list of all clients
 * client_list_t *current
 *	- the current client trying to check the online status of another user
 *
 * return
 *	- N/A
 */
void validate_user(char * destination, client_list_t * clientList, client_list_t * current) {
	char new_buffer[BUFFER_SIZE];
	client_list_t *list = clientList;
	while(list != NULL) { // search list for username
		if(strcmp(destination, list->username)==0) { //username matches name in list
			sprintf(new_buffer, "13%c %c %c %cY", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
			send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
			return;
		}
		else
			list = list->next;
	}
	sprintf(new_buffer, "13%c %c %c %cN", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;
}



/* This function will confirm to the current client whether another user exists,
 * by sending a mode 14 message, with a character in the body of the message.
 * Y = yes, the user exists
 * N = no, the user does not exist
 *
 * char *destination
 *	- the username that the client is checking to see exists
 * client_list_t *current
 *	- A pointer to the structure identifying client asking the server about the user
 *
 * return
 *	- N/A
 */
void confirm_existence(char * destination, client_list_t * current){
	FILE * fp;
	fp = fopen("logins.txt", "r");
	char search[3];
	char temp[BUFFER_SIZE];
	char * token;
	char new_buffer[BUFFER_SIZE];
	search[2]='\0';
	search[1]='\n';
	search[0]=DELIMITER;
	sprintf(new_buffer, "14%c %c %c %c", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(fgets(temp, BUFFER_SIZE, fp)!=NULL){
		token = strtok(temp, search);
		if(strcmp(token, destination)==0){
			//a match was found
			strcat(new_buffer, "Y");
			send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
			fclose(fp);
			return;
		}
		clear_string(temp, BUFFER_SIZE);
	}	
	//a match was not found
	strcat(new_buffer, "N");
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	fclose(fp);
	return;
}



/* This function will send a file, named in body, and the client identified
 * in current, so long as the file is meant for the current user. If not,
 * it behaves as if the file does not exit.
 *
 * char *body
 *	- A string containing the new username for the client
 * client_list_t *current
 *	- A pointer to the structure identifying client changing their password
 *
 * return
 *	- N/A
 */
void change_password(char * body, client_list_t * current) {
	FILE * fp;
	char search[3];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';
	char * token;
	char temp[BUFFER_SIZE];
	char username[CREDENTIAL_SIZE];
	char password[CREDENTIAL_SIZE];
	char new_contents[BUFFER_SIZE];
	char is_banned[CREDENTIAL_SIZE];
	clear_string(new_contents, BUFFER_SIZE);
	fp = fopen("logins.txt", "r");
	while (fgets(temp, BUFFER_SIZE, fp) != NULL) {
		token = strtok(temp, search);
		strcpy(username, token);
		token = strtok(NULL, search);
		strcpy(password, token);
		token = strtok(NULL, search);
		strcpy(is_banned,token);
		clear_string(temp, BUFFER_SIZE);
		if(strcmp(username, current->username)) {
			sprintf(temp, "%s%c%s%c%s\n", username, (char)DELIMITER, password, (char)DELIMITER, is_banned);
			strcat(new_contents, temp);
		}
	}
	fclose(fp);
	fopen("logins.txt", "w");
	fprintf(fp, "%s", new_contents);
       	fprintf(fp, "%s%c%s%c%s", current->username, (char)DELIMITER, body, (char)DELIMITER, current->is_banned);
	fclose(fp);
	strcpy(current->password, body);
	sprintf(temp, "4%c %c %c %c ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, temp, strlen(temp), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;
}



/* This function will send a message to the current client being processed,
 * containing a list of all registered users.
 *
 * client_list_t *current
 *	- A pointer to the structure identifying the client asking for a list of users
 *
 * return
 *	- N/A
 */
void show_all_users(client_list_t *current) {
	FILE * fp;
	char temp[BUFFER_SIZE];
	char search[3];
	char * token;
	char new_buffer[BUFFER_SIZE];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';
	sprintf(new_buffer, "17%c %c %c %cLIST OF ALL USERS: ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	fp = fopen("logins.txt", "r");
	while(fgets(temp, BUFFER_SIZE, fp) != NULL) {
		token = strtok(temp, search);
		strcat(new_buffer, token);
		strcat(new_buffer, " ");
	}
	fclose(fp);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;
}

/* This function will accept the binary data of a file, and store
 * it in a file, denoted by the filename given in the body of the
 * last message.
 *
 * char *body
 *	- a string containing the filename and file size, seperated by an underscore
 * char *destination
 *	- the user that the file is intended for
 * client_list_t *current
 *	- the user that is sending the file
 *
 * return
 *	- N/A
 */
void recieve_file(char *body, char *destination, client_list_t *current){
	char buffer[BUFFER_SIZE];
	char *filename;
	char *str_size;
	unsigned long size;
	int amount_read;

	// splice the "body" at the last occuring underscore,
	// which will give us the file's name, and size
	int i=0;
	while(body[i]!='\0')
		i++;
	while(body[i]!='_')
		i--;
	body[i] = '\0';
	filename = body;
	str_size = &body[i+1];

	size = atoul(str_size);

	// recieve the actual file data from the client
	FILE *fp = fopen(filename, "w");
	while(size>0){
		amount_read = recv(current->socket , buffer, BUFFER_SIZE, MSG_NOSIGNAL);
		size-=amount_read;
		fwrite(buffer, 1, amount_read, fp);
	}
	fclose(fp);

	// append this file to the list of files
	fp = fopen("filelist.txt", "a");
	fprintf(fp, "%s%c%s%c%s\n", destination, (char)DELIMITER, str_size, (char)DELIMITER, filename);
	fclose(fp);
}


/* This function will convert a string of numbers to an
 * unsigned long, and return this value to the user.
 *
 * char *value
 *	- pointer to a string of numbers
 * return
 *	- unsigned long denoting the numerical value of the string
 */
unsigned long atoul(char *value){
	int i=0;
	unsigned long number = 0;
	while(value[i]!='\0'){
		number *= 10;
		number += value[i] - '0';
		i++;
	}
	return number;
}



/* This function will communicate a list of files, meant for the current
 * client being processed,to said client. If there are no files meant for
 * the user, then the user will be told so.
 *
 * client_list_t *current
 *	- A pointer to the structure identifying the current client being processed
 *
 * return
 *	- N/A
 */
void show_user_files(client_list_t *current){
	char readBuffer[BUFFER_SIZE];
	char sendBuffer[BUFFER_SIZE];
	sendBuffer[0]='\0';
	char search[3];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';
	char *destination;
	char *filename;
	int filesFound=0;

	// print the header of the message to the sending buffer
	sprintf(sendBuffer,"18%c%s%c%s%c%s%c", (char)DELIMITER, current->username, (char)DELIMITER, " ", (char)DELIMITER, " ", (char)DELIMITER);

	// open the list of files, to check if there are any files available
	FILE *fp = fopen("filelist.txt", "r");
	if (fp != NULL){
		// read a row at a time
		while (fgets(readBuffer, BUFFER_SIZE, fp) != NULL){
			// tokenize the line read into destination and filename
			destination = strtok(readBuffer, search);
			strtok(NULL, search);
			filename = strtok(NULL, search);
			// check if the destination matches the current client's username, and if so, append file to buffer
			if (strcmp(destination, current->username)==0){
				strcat(sendBuffer, filename);
				strcat(sendBuffer, "\n");
				filesFound=1;
			}
		}
		fclose(fp);
	}

	// if the file list does not exist, or no files are found, append a message saying so to the buffer
	if (!filesFound || fp == NULL){
		strcat(sendBuffer, "\nNO FILES WERE FOUND FOR YOU\n\n");
	}

	// send the buffered message to the current client
	send(current->socket, sendBuffer, strlen(sendBuffer)+1, MSG_NOSIGNAL | MSG_DONTWAIT);

	return;
}



/* This function will send a file, named in body, and the client identified
 * in current, so long as the file is meant for the current user. If not,
 * it behaves as if the file does not exit.
 *
 * char *body
 *	- A string containing the name of the file being requested
 * client_list_t *current
 *	- A pointer to the structure identifying the current client being processed
 *
 * return
 *	- N/A
 */
void send_file(char *body, client_list_t *current){
	char readBuffer[BUFFER_SIZE];
	char sendBuffer[BUFFER_SIZE];
	int sendSize;
	sendBuffer[0]='\0';
	char *destination;
	char *filename;
	char *str_filesize;
	unsigned long filesize;
	char search[3];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';
	int fileFound=0;

	// check the file list, and ensure that the requested file exists, and is meant for the requesting user
	FILE *fp = fopen("filelist.txt", "r");
	if (fp != NULL){
		while (fgets(readBuffer, BUFFER_SIZE, fp) != NULL){
			destination = strtok(readBuffer, search);
			str_filesize = strtok(NULL, search);
			filename = strtok(NULL, search);
			if (strcmp(destination, current->username)==0 && strcmp(body, filename)==0){
				fileFound=1;
				break;
			}
		}
		fclose(fp);
	}

	// the file exists in the file list, now try to load it
	if (fileFound){
		// convert the filesize string to an unsigned long
		filesize = atoul(str_filesize);

		// open the file to be sent
		fp = fopen(filename, "r");
		if (fp!=NULL){
			// if the file was successfully opened, begin the sending process
			printf("SENDING FILE %s TO %s\n", filename, current->username);

			// send initial message, with the body containing filename_filesize
			sprintf(sendBuffer, "10%c%s%c %c %c%s_%s", (char)DELIMITER, current->username, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, filename, str_filesize);
			send(current->socket, sendBuffer, strlen(sendBuffer)+1, MSG_NOSIGNAL | MSG_DONTWAIT);

			// read the file in buffer sized chunks, and send it to the client
			do{
				sendSize = fread(sendBuffer, 1, BUFFER_SIZE, fp);
				filesize -= sendSize;
				send(current->socket, sendBuffer, sendSize, MSG_NOSIGNAL | MSG_DONTWAIT);
			}while(filesize>0);

			// close the file
			fclose(fp);
			printf("FILE SUCCESSFULLY SENT\n");
		}
		else{
			// if the file could not be opened, reset the fileFound variable
			fileFound = 0;
		}
	}

	// if the file was not found, echo the file request to the client
	if (!fileFound){
		printf("FILE NOT FOUND\n");
		sprintf(sendBuffer, "19%c%s%c %c %c ", (char)DELIMITER, current->username, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
		send(current->socket, sendBuffer, strlen(sendBuffer)+1, MSG_NOSIGNAL | MSG_DONTWAIT);
	}

	return;
}
