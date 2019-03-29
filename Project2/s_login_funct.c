//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_login_funct.c

#include "Definitions.h"
#include "s_login_funct.h"

int login_user(char *username, char *password, client_list_t *client){
	FILE *logins = fopen("logins.txt", "r");
	char login_buffer[200];

	char search[3];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';

	char *load_username=NULL;
	char *load_password=NULL;
	char *is_banned=NULL;

	// check to see if the given credentials match existing records
	while(!feof(logins)){
		fgets(login_buffer, 200, logins);
		load_username = strtok(login_buffer, search);
		if (load_username!=NULL) 
			load_password = strtok(NULL, search);
		if (load_username!=NULL && load_password!=NULL){
			if (strcmp(load_username, username)==0 && strcmp(load_password, password)==0){
				is_banned=strtok(NULL, search);	
				fclose(logins);
				char new_buffer[BUFFER_SIZE];
				if(atoi(is_banned) != 1) {
					char new_buffer[BUFFER_SIZE];
					sprintf(new_buffer, "2%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
					send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
					strcpy(client->username, username);
					strcpy(client->password, password);
					strcpy(client->is_banned, is_banned);
					client->logged_in=1;
					return 1;
				}
				else {//user is banned, notify client
					sprintf(new_buffer, "2%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, "1");
					send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
					printf("%s IS BANNED NOTIFYING CLIENT\n", username);
					return -1;
				}
			}
		}
	}

	// the given credentials did not match any records
	fclose(logins);

	char new_buffer[BUFFER_SIZE];
	sprintf(new_buffer, "1%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
	send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);

	return 0;
}



int register_user(char *username, char *password, client_list_t *client){
	FILE *logins = fopen("logins.txt", "r");
	char login_buffer[200];

	char search[3];
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';

	char *load_username;

	// check to see if the given username exists already
	while(!feof(logins)){
		fgets(login_buffer, 200, logins);
		load_username = strtok(login_buffer, search);
		if (load_username!=NULL){
			if (strcmp(load_username, username)==0){
				fclose(logins);
				char new_buffer[BUFFER_SIZE];
				sprintf(new_buffer, "0%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
				send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
				return 1;
			}
		}
	}

	// the given username does not exist in the records, create new account
	fclose(logins);
	logins = fopen("logins.txt", "a");
	fprintf(logins, "%s%c%s%c0\n", username, (char)DELIMITER, password, (char)DELIMITER);
	fclose(logins);

	char new_buffer[BUFFER_SIZE];
	sprintf(new_buffer, "2%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
	send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	strcpy(client->username, username);
	strcpy(client->password, password);
	strcpy(client->is_banned, "0");
	client->logged_in=1;

	return 0;
}
void s_logout(char * username, char * password, client_list_t *client){
	if (strcmp(client->username, username)==0 && strcmp(client->password, password)==0){
		char new_buffer[BUFFER_SIZE];
		sprintf(new_buffer, "3%c %c %c %c ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
		send(client->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		printf("Logging out %s\n", username);
		client->logged_in=0;
		strcpy(client->username, "\0");
		strcpy(client->password, "\0");
	}
	return;
}
void check_if_banned(char * username, client_list_t * current){
	FILE * fp;
	char * token;
	char temp[BUFFER_SIZE];
	char search[3];
	int done = 0;
	search[0]=(char)DELIMITER;
	search[1]='\n';
	search[2]='\0';
	fp = fopen("logins.txt", "r");
	while((fgets(temp, BUFFER_SIZE, fp) != NULL) && !done) {
		token = strtok(temp, search);
		if(!strcmp(username, token)) {//username matches currently read username
			token = strtok(NULL, search);
			token = strtok(NULL, search);
			if(atoi(token)==1)//user is banned
				sprintf(temp, "15%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
			else //user is not banned
				sprintf(temp, "15%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
			send(current->socket, temp, strlen(temp), MSG_NOSIGNAL | MSG_DONTWAIT);
		}			
	}
	fclose(fp);
	return;
}
