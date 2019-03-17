#include "Definitions.h"
#include "c_menu_funct.h"

void group_chat(server_t *server){
	char input;
	printf("\n-=| GROUP CHAT |=-");
	printf("\n-=| HIT ENTER TO TYPE A MESSAGE |=-");
	printf("\n-=| PRESS Q THEN ENTER TO EXIT |=-\n\n");
	server->in_group_chat=1;
	do{
		input=getchar();
		if (input!='Q' && input!='q'){
			while(server->send==1);
			// mutex 1 lock to replace typing variable
			server->typing=1;
			printf("YOUR MESSAGE: ");
			sprintf(server->buffer_out,"7%c%s%c%s%c %c", (char)DELIMITER, server->username, (char)DELIMITER, server->password, (char)DELIMITER, (char)DELIMITER);
			fgets(&server->buffer_out[strlen(server->buffer_out)], BUFFER_SIZE-strlen(server->buffer_out), stdin);
			server->buffered_out_size=strlen(server->buffer_out)+1;
			server->send=1;
			server->typing=0;
			// mutex 1 unlock to replace typing variable
		}

		if (input!='\n')
			while(getchar()!='\n');
	}while(input != 'q' && input !='Q');
	server->in_group_chat=0;
}