/*
 * Server side C program that echos
 * messages back to the client program
 *
 * @AUTHOR	Curtis Duvall, Nathan Wilkins, Julian Thayer
 * @DATE	3/8/2019
 * @INFO	COP4635 Project 1
 */



#include "server_framework.h" 
#include "s_admin_funct.h" 



int main(int argc, char const *argv[])
{
    int server_fd;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int valid=0;
    char verify_password[CREDENTIAL_SIZE];

    // create the root of the client link list,
    // for keeping track of the clients.
    client_list_t *clientList = (client_list_t *) malloc(sizeof(client_list_t));
    clientList->socket=0;
    clientList->connected=0;
    clientList->logged_in=0;
    clientList->last=NULL;
    clientList->next=NULL;
    clientList->username[0]='\0';
    clientList->password[0]='\0';
    clientList->last_reception =(struct timeval *) malloc(sizeof(struct timeval));
    clientList->ping=0;

    // initialize the thread for pinging the clients
    pthread_t tid;
    if (pthread_create(&tid, NULL, ping_connections, (void *)clientList)){
	fprintf(stderr,"ERROR: could not create ping thread\n");
    }

    // allocate memory for tracking administrator account
    admin_account_t *admin = (admin_account_t *) malloc(sizeof(admin_account_t));

    // setup values for the address
    memset(&address, '\0', sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // create the administrative login credentials before the system fully boots up
    do {
	    printf("Please enter the administrative account's username: ");
	    fgets(admin->username, CREDENTIAL_SIZE, stdin);
	    admin->username[strlen(admin->username)-1]='\0';
	    printf("Please enter the administrative account's password: ");
	    fgets(admin->password, CREDENTIAL_SIZE, stdin);
	    printf("Please Re-enter the administrative account's password: ");
	    fgets(verify_password, CREDENTIAL_SIZE, stdin);
	    if(strcmp(verify_password, admin->password)!=0) 
		fprintf(stderr, "ERROR: PASSWORDS DID NOT MATCH\n");
	    else if(strlen(admin->username)<=0)
		fprintf(stderr, "ERROR: USERNAME CANNOT BE NULL\n");
	    else if(strlen(admin->password)<=1)
		fprintf(stderr, "ERROR: PASSWORD CANNOT BE NULL\n");
	    else{
		valid =1;
		admin->password[strlen(admin->password)-1]='\0';	
	    }
    }while(valid != 1);

    // Ensure there is a login registry
    FILE *logins = fopen("logins.txt", "a");
    fclose(logins);

    // Creating socket file descriptor
    printf("STATUS: creating socket file descriptor\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

/*
************************************MY EDITS *******************************************
********************************allow immediate reuse of server socket******************
*/	
    int on=1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
//***************************************************************************************

    // bind the socket to the address, and ensure that there are no errors
    printf("STATUS: binding socket to address\n");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // setup the server to listen for new connections
    printf("STATUS: setting server to listen for new connections\n");
    if (listen(server_fd, 20) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // make it so that future calls to accept
    // new sockets do not block. This means
    // that the server will keep going if
    // there is no new client to connect to.
    int temp_socket;
    int flags = fcntl(server_fd, F_GETFL);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    while(1)
    {
	// buffer for reading messages from the clients
	char buffer[BUFFER_SIZE];

	// variables to hold the data in the fields of the messages recieved
	int mode;
	char body[BUFFER_SIZE];
	char username[CREDENTIAL_SIZE];
	char password[CREDENTIAL_SIZE];
	char destination[CREDENTIAL_SIZE];

        // Here we will check to see if there is
        // a new connection request, and if there
        // is, we will add it to the list.
	temp_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (temp_socket>0)
            new_connection(clientList, temp_socket);

	// loop through every connection, to
        // see if there is an incoming message.
        // This must be done for any type of
        // message; public, private, file, etc.
        client_list_t *current = clientList;
        while (current!=NULL){
                if (current->connected==1){
                    //clear buffer and path array for message acceptance
                    clear_string(buffer, BUFFER_SIZE);

                    // accept message from client
                    valread = recv( current->socket , buffer, BUFFER_SIZE, MSG_NOSIGNAL | MSG_DONTWAIT);

                    // handle recieved messages
                    if (valread>0){
			    
			// a message has been recieved, update the time of last reception
			gettimeofday(current->last_reception,NULL);
			    
			// parse the recieved message into seperate data fields
			parse_message(buffer, &mode, username, password, destination, body);

			// perform specific actions based on the mode value
			switch (mode){
				case 0: // register user
					printf("Verifying registration of '%s'\n", username);
					register_user(username, password, current);
					break;
				case 1: // login user
					printf("Verifying login credentials of '%s'\n", username);
					login_user(username, password, current);
					break;
				case 2:
					// THIS CASE IS RESERVED FOR MESSAGES FROM THE
					// SERVER TO THE CLIENT, TO VERIFY LOGIN SUCCESS
					break;
				case 3: //logout case
					s_logout(username, password, current);
					break;
				case 4: // change password
					printf("Changing Password of '%s' to '%s'\n", current->username, body);
					change_password(body, current);
					break;
				case 5: //online user query case
					printf("Sending '%s' a list of all online users\n", current->username);
					show_users(username, password, clientList, current);
					break;
				case 6: // private message
					private_message(clientList, body, destination, current->username);
					break;
				case 7: // group broadcast
					if (current->logged_in==1){
						if (strcmp(destination, " ")==0){
							broadcast_message(clientList, current->socket, body, current->username);
							log_into_group(username,body);
						}
					}
					break;
				case 8: // send group log
					printf("Sending group chat log to '%s'\n", current->username);
					send_group_log(current);
					break;
				case 9: //send private log
					printf("Sending private chat log between '%s' and '%s' to '%s'\n", destination, current->username, current->username);
					send_private_log(destination, current);
					break;
				case 10:// Recieve files from client
					printf("Recieving file from %s\n", current->username);
					recieve_file(body, destination, current);
					break;
				case 11: // ban user
					printf("Banning user '%s'\n", destination);
					ban_user(destination, clientList, current); 
					break;
				case 12: //kick user
					printf("Kicking user '%s'\n", destination);
					kick_user(destination, clientList, current);
					break;
				case 13: // validate user is online
					printf("Validating if '%s' is online\n", destination);
					validate_user(destination, clientList, current);
					break;
				case 14: // case of confirming user exists
					printf("Validating that '%s' is an existing user for %s.\n", destination, current->username);
					confirm_existence(destination, current);
					break;
				case 15: // check if user is banned
					printf("Checking if '%s' is banned.\n", username);
					check_if_banned(username, current);
					break;
				case 16: //login as admin
					printf("Verifying admin credentials from '%s'.", current->username);
					admin_login(username, password, current, admin);
					break;
				case 17: //show all users
					printf("Sending list of all users to '%s'\n", current->username);
					show_all_users(current);
					break;
				case 18: // list all files meant for the user
					show_user_files(current);
					break;
				case 19: // The client has requested a file
					send_file(body, current);
					break;
				case 20: // send a ping to the current client
					printf("STATUS: ping response recieved from '%s'\n", current->username);
					break;
			}
                    }// if there is no message from the client, and ping is set
                    else if (current->ping==1){
			// send a simple ping message to the client
                        char pingMessage[CREDENTIAL_SIZE];
			sprintf(pingMessage,"20%c %c %c %c ", (char)DELIMITER, (char)DELIMITER, (char)DELIMITER, (char)DELIMITER);
                        send(current->socket , pingMessage , strlen(pingMessage), MSG_NOSIGNAL | MSG_DONTWAIT);
			current->ping=0;
			printf("STATUS: ping sent to '%s'\n", current->username);
                    }

                    // ensure that it has been less than TIMEOUT_INTERVAL since
                    // the last message recieved from the current client. If it
                    // has been longer, then the client has most likely disconnected,
                    // and should be removed.
                    struct timeval time;
                    gettimeofday(&time, NULL);
                    if ((time.tv_sec - current->last_reception->tv_sec)>TIMEOUT_INTERVAL){
			current->connected=0;
                        remove_connection(clientList, current->socket);
                    }
                }
            // move to the next element in the list
            current = current->next;
        }
    }
    // Shut everything down.
    // Because a server must continually run,
    // this portion should never be reached.
    pthread_join(tid, NULL);
    close(server_fd);
    free(admin);
    printf("STATUS: connection broken\n");
    return 0;
}



/* This function will add a new connection socket to the link list, so
 * that the server can loop through all connections.
 *
 * int socket
 *	- the integer identifier of the socket connected to the client
 *
 * client_list_t *clientList
 *	- link list of all the client connections
 */
void new_connection(client_list_t *clientList, int socket){
    // there is at least 1 connection
    if (clientList->connected == 1){
        client_list_t *end = clientList;
        while(end->next != NULL)
            end = end->next;
        end->next = (client_list_t *)malloc(sizeof(client_list_t));
        end->next->next = NULL;
        end->next->last = end;
        end->next->socket = socket;
        end->next->connected = 1;
	end->next->logged_in=0;
	end->next->username[0]='\0';
	end->next->password[0]='\0';
	end->next->last_reception =(struct timeval *) malloc(sizeof(struct timeval));
	gettimeofday(end->next->last_reception, NULL);
	end->next->ping = 0;
    }
    else{ // there are no connections
        clientList->socket = socket;
        clientList->connected = 1;
	clientList->logged_in=0;
	clientList->username[0]='\0';
	clientList->password[0]='\0';
	gettimeofday(clientList->last_reception, NULL);
	clientList->ping = 0;
    }
    printf("STATUS: new connection added, socket:%d\n", socket);

}



/* This function will remove the connection with the target_socket
 * from the list of clients.
 *
 * int target_socket
 *	- an integer identifying the socket to remove from the connection list
 *
 * client_list_t * clientList
 *	- a link list of all the client connections
 */
void remove_connection(client_list_t *clientList, int target_socket){
    // special case, where the target being
    // removed is the first element in the
    // link-list.
    printf("STATUS: removing connection at socket '%d'\n", target_socket);
    if (clientList->socket == target_socket){
        clientList->socket = 0;
        clientList->connected = 0;
        clientList->logged_in = 0;
        clientList->username[0]='\0';
        clientList->password[0]='\0';
	clientList->ping=0;
        return;
    }

    // look for the correct element in the link list
    client_list_t *target = clientList;
    while(target->socket != target_socket && target != NULL)
        target = target->next;

    // if the element exists, remove it from the link list
    if (target != NULL){
        target->last->next = target->next;
	if (target->next != NULL)
		target->next->last = target->last;
        free(target);
    }
}



/* This is a thread function, which will set the server to ping all the
 * clients every half a timeout interval.
 *
 * void *vargp
 *	- a pointer to the root of the link list containing all the client data
 */
void *ping_connections(void *vargp){
	while(1){
		client_list_t *current = (client_list_t *)vargp;
		while (current!=NULL){
			if (current->connected==1 && current->ping==0){
				current->ping=1;
			}
			current=current->next;
		}
		sleep(TIMEOUT_INTERVAL/2);
	}
	return NULL;
}
