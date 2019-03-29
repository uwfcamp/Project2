//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//s_admin_funct.c

#include "Definitions.h"
#include "s_admin_funct.h"
#include "parse.h"

/********************************************************************
 * this function loggs a client in as an admin verifying correct admin
 * username and password combination.
 *******************************************************************/
void admin_login(char * username, char * password, client_list_t *current, admin_account_t * admin){
	char new_buffer[BUFFER_SIZE];
	if(!strcmp(username, admin->username) && !strcmp(password, admin->password)) // username and password match
		sprintf(new_buffer, "16%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	else// username and password dont match
		sprintf(new_buffer, "16%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	//send response to server
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT); 
	return;
}

/************************************************************************
 * This function takes the input response from the client and sends an
 * an acknowledgement to the admin and a ban message to the client. The
 * program then goes through the logins.txt file to changed the is_banned
 * field (the last value) from a '0' to a '1'.
 ***********************************************************************/
void ban_user(char * destination, client_list_t *clientList, client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	client_list_t * temp;
	//notify admin that client is being banned
	sprintf(new_buffer, "11%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);

	//send ban messaget to banned user
	sprintf(new_buffer, "11%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	while(temp != NULL)
	{
		if(strcmp(temp->username, destination)==0)
			send(temp->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		temp = temp->next;
	}
	//search through logins.txt to make is_banned field true
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
	//print logins file to the buffer channging the banned is_banned state to true.
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
	//create updated logins.txt file
	fp = fopen("logins.txt", "w");
	fprintf(fp, "%s", new_buffer);
	fclose(fp);
	printf("%s WAS BANNED\n", destination);
	return;
}
/*******************************************************************************
 * This function will take the input from the admin and acknowledge to the admin
 * that it recieved its command. It will search through the active clients and 
 * send out the kick message to the appropriate client.
 ******************************************************************************/
void kick_user(char * destination, client_list_t *clientList, client_list_t *current) {
	char new_buffer[BUFFER_SIZE];
	client_list_t * temp = clientList;
	//notify admin that client is being kicked
	sprintf(new_buffer, "12%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);

	//kick functionality here
	sprintf(new_buffer, "12%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	//find the correct client to be kicked and send him the kicked message.
	while(temp != NULL)
	{
		if(strcmp(temp->username, destination)==0)
			send(temp->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
		temp = temp->next;
	}
	printf("%s WAS KICKED\n", destination);
	return;
}
