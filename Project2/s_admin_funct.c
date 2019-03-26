#include "Definitions.h"
#include "s_admin_funct.h"

void admin_login(char * username, char * password, client_list_t *current, admin_account_t * admin){
	char new_buffer[BUFFER_SIZE];
	if(!strcmp(username, admin->username) && !strcmp(password, admin->password))
		sprintf(new_buffer, "16%c %c %c %c1", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	else
		sprintf(new_buffer, "16%c %c %c %c0", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
	send(current->socket, new_buffer, strlen(new_buffer), MSG_NOSIGNAL | MSG_DONTWAIT);
	return;
}
