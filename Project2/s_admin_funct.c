#include "Definitions.h"
#include "s_admin_funct.h"
#include "parse.h"

void admin_login(char * username, char * password, client_list_t *current, admin_account_t * admin){
	char new_buffer[BUFFER_SIZE];
	if(!strcmp(username, admin->username) && !strcmp(password, admin->password))
		sprintf(new_buffer, "16%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	else
		sprintf(new_buffer, "16%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;
}
void ban_user(char * destination, client_list_t *clientList, client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	client_list_t * temp;
	//notify admin that client is being banned
	sprintf(new_buffer, "11%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);

	//insert ban functionality here
	sprintf(new_buffer, "11%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(temp != NULL)
	{
		if(strcmp(temp->username, destination)==0)
			send(temp->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		temp = temp->next;
	}
	FILE * fp;
	char * token;
	char search[3];
	char temp_buff[BUFFER_SIZE];
	char user[CREDENTIAL_SIZE];
	char pass[CREDENTIAL_SIZE];
	char is_banned[CREDENTIAL_SIZE];
	clear_string(new_buffer, BUFFER_SIZE);
	search[0] = (char)DELIMITER;
	search[1] = '\n';
	search[2] = 0;
	fp = fopen("logins.txt", "r");
	while(fgets(temp_buff, BUFFER_SIZE, fp) != NULL) {
		token = strtok(temp_buff, search);
		strcpy(user, token);
		token = strtok(NULL, search);
		strcpy(pass, token);
		token = strtok(NULL, search);
		strcpy(is_banned, token);
		if(strcmp(user, destination)==0) 
			sprintf(temp_buff, "%s%c%s%c1\n", user, (char)DELIMITER, pass, (char)DELIMITER); 
		else	
			sprintf(temp_buff, "%s%c%s%c%s\n", user, (char)DELIMITER, pass, (char)DELIMITER, is_banned); 
		strcat(new_buffer, temp_buff);
	}
	fclose(fp);
	fp = fopen("logins.txt", "w");
	fprintf(fp, "%s", new_buffer);
	fclose(fp);
	printf("%s was banned\n", destination);
	return;
}
void kick_user(char * destination, client_list_t *clientList, client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	client_list_t * temp = clientList;
	//notify admin that client is being kicked
	sprintf(new_buffer, "12%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);

	//insert kick functionality here
	sprintf(new_buffer, "12%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(temp != NULL)
	{
		if(strcmp(temp->username, destination)==0)
			send(temp->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		temp = temp->next;
	}
	printf("%s was kicked\n", destination);
	return;
}
