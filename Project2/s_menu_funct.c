#include "Definitions.h"
#include "parse.h"
#include "s_menu_funct.h"



void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender){
    char new_buffer[BUFFER_SIZE];
    sprintf(new_buffer, "7%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, " ", (char)DELIMITER, message);
    while(clientList != NULL){
        if (clientList->socket != sender_socket && clientList->connected==1 && clientList->logged_in==1){
	        send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
        clientList=clientList->next;
    }
    printf("STATUS: message sent to all users\n");
}



void private_message(client_list_t *clientList, char *message, char *destination, char *sender){
    int i=0;
    char new_buffer[BUFFER_SIZE];
    log_into_private(sender, destination, message);
    sprintf(new_buffer, "6%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, destination, (char)DELIMITER, message);
    while(clientList != NULL){
        if (strcmp(destination, clientList->username)==0){
		if (clientList->connected==1 && clientList->logged_in==1){
	        	send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
                	printf("STATUS: message sent to %s\n", destination);
			i++;
		}
	}
        clientList=clientList->next;
    }
    if(i==0)
    	printf("STATUS: message could not be sent to %s\n", destination);
}

void log_into_group(char *username, char *body){
	FILE *fp;
	fp=fopen("groupchat.txt", "a");

	time_t rawtime;
	time(&rawtime);
	struct tm *info = localtime(&rawtime);
	char timestamp[MAX_TIME_SIZE];
	clear_string(timestamp,MAX_TIME_SIZE);
	strcpy(timestamp,asctime(info));
	timestamp[strlen(timestamp)-1]='\0'; // removing newline

	fprintf(fp, "%s - %s: %s",timestamp, username, body);

	fclose(fp);
	return;
}


void log_into_private(char *sender, char *reciever, char *body){
	FILE *fp;
	fp=fopen("privatechat.txt","a");

	time_t rawtime;
	time(&rawtime);
	struct tm *info = localtime(&rawtime);
	char timestamp[MAX_TIME_SIZE];
	clear_string(timestamp,MAX_TIME_SIZE);
	strcpy(timestamp,asctime(info));
	timestamp[strlen(timestamp)-1]='\0'; // removing newline

	fprintf(fp, "%s%c%s%c%s%c%s", timestamp, (char)DELIMITER, sender, (char)DELIMITER, reciever, (char)DELIMITER, body);

	fclose(fp);
	return;
}



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
 *
 * char *destination
 *	- the user that the file is intended for
 *
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
 *
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
