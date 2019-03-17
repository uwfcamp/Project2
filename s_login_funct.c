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

	// check to see if the given credentials match existing records
	while(!feof(logins)){
		fgets(login_buffer, 200, logins);
		load_username = strtok(login_buffer, search);
		if (load_username!=NULL)
			load_password = strtok(NULL, search);
		if (load_username!=NULL && load_password!=NULL){
			if (strcmp(load_username, username)==0 && strcmp(load_password, password)==0){
				fclose(logins);
				char new_buffer[BUFFER_SIZE];
				sprintf(new_buffer, "2%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
				send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
				strcpy(client->username, username);
				strcpy(client->password, password);
				client->logged_in=1;
				return 1;
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
	fprintf(logins, "%s%c%s\n", username, (char)DELIMITER, password);
	fclose(logins);

	char new_buffer[BUFFER_SIZE];
	sprintf(new_buffer, "2%c%s%c%s%c%s%c%s",(char)DELIMITER, username, (char)DELIMITER, password, (char)DELIMITER, " ", (char)DELIMITER, " ");
	send(client->socket , new_buffer , strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	strcpy(client->username, username);
	strcpy(client->password, password);
	client->logged_in=1;

	return 0;
}