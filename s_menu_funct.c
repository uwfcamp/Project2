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
    sprintf(new_buffer, "7%c%s%c%s%c%s%c%s", (char)DELIMITER, sender, (char)DELIMITER, " ", (char)DELIMITER, destination, (char)DELIMITER, message);
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
