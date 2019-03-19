#include "Definitions.h"
#include "parse.h"
#include "s_menu_funct.h"



void broadcast_message(client_list_t *clientList, int sender_socket, char *message, char *sender){
    char new_buffer[BUFFER_SIZE];
    sprintf(new_buffer, "7%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, " ", (char)DELIMITER, message);
    while(clientList != NULL){
        if (clientList->socket != sender_socket && clientList->connected==1 && clientList->logged_in==1){
	        send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		break;
	}
        clientList=clientList->next;
    }
    printf("STATUS: message sent to all users\n");
}



void private_message(client_list_t *clientList, char *message, char *destination, char *sender){
    char new_buffer[BUFFER_SIZE];
    sprintf(new_buffer, "6%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, destination, (char)DELIMITER, message);
    while(clientList != NULL){
        if (strcmp(destination, clientList->username)==0){
		if (clientList->connected==1 && clientList->logged_in==1)
	        	send(clientList->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
                printf("STATUS: message sent to %s\n", destination);
		return;
	}
        clientList=clientList->next;
    }
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
	sprintf(new_buffer, "5%c %c %c %cCURRENT USER LIST: ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(list != NULL) {
		if(list->logged_in == 1){
			strcat(new_buffer, list->username);
			strcat(new_buffer, " ");
			i++;
		}
		list = list->next;
	}
	strcat(new_buffer, "\nCURRENT NUMBER OF USERS: ");
	sprintf(temp, "%d\n", i);
	strcat(new_buffer, temp);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;	
}



void send_group_log(client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
	sprintf(new_buffer, "5%c %c %c %c\n", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	FILE * fp;
	fp = fopen("groupchat.txt", "r");
	while(fgets(temp, BUFFER_SIZE, fp)!=NULL)
		strcat(new_buffer, temp);	
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
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
