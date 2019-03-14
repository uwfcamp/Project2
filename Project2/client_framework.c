// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



#define PORT 60001
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 30000



typedef struct server_s{
	int socket;		// identifier for the server socket
	int header[MAX_PATH_ID_LENGTH]; // holds 'header' information for program routing
	char *buffer;		// Pointer to the buffer
	int buffer_size;	// The max size of buffer
	int buffered_size;	// The number of bytes in buffer
	int send;		// 0 if there is nothing to be sent, 1 if there is
	int recieve;		// 0 if nothing has been recieved, 1 if something has
	int connected;		// 0 if not connected, 1 if connected
}server_t;



server_t *build_server_structure(void);
void disconnect(server_t *server);
void *server_communication(void *vargp);

int main(int argc, char const *argv[])
{
    long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    server_t *server = build_server_structure();


    if ((server->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(server->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(1){
        // get user's input
        printf("PATH: ");
        server->buffer[0]='\0';
        server->buffered_size=0;
        fgets(server->buffer, BUFFER_SIZE, stdin);
	printf("MESSAGE: ");
	fgets(&server->buffer[strlen(server->buffer)], BUFFER_SIZE-strlen(server->buffer), stdin);
        // establish size of input string
        server->buffered_size=strlen(server->buffer)+1;
        // declare there is a message to send to the server
	server->send=1;

	server_communication((void *)server);

	if (server->recieve==1)
	        printf("RESPONSE: %s\n",server->buffer );
        //while (getchar()!='\n');
    }

    disconnect(server);

    return 0;
}



// This function will check for a message that
// needs to be sent and a message that is
// being recieved.
void *server_communication(void *vargp){
	//while(1){

	// send message to server
	server_t *server = (server_t *)vargp;

	//send message to server
	if (server->send==1){
		if (send(server->socket , server->buffer, server->buffered_size , MSG_NOSIGNAL)<0)
			server->connected=0;
		server->send=0;
	}

	// recieve message from server
	if (server->recieve==0){
		server->buffered_size = recv(server->socket, server->buffer, server->buffer_size, MSG_NOSIGNAL);	

		int err = errno;

		//check if any data was actually recieved
		if (server->buffered_size>0)
			server->recieve=1;
		else if (err == EAGAIN || err == EWOULDBLOCK)
			server->buffered_size=0;
		else if (server->buffered_size<0)
			server->connected=0;
	}

	//}
	return NULL;
}



// This function builds the stucture used for
// communication to and from the server
server_t *build_server_structure(void){
	server_t *server = (server_t *)malloc(sizeof(server_t));
	server->socket = 0;
	server->buffer_size=BUFFER_SIZE;
	server->buffer=(char *)malloc(BUFFER_SIZE);
	server->buffered_size=0;
	server->send=0;
	server->recieve=0;
	server->connected=0;

	return server;
}



// This function disconnects from the server
// and frees up the memory allocated for
// communication. Should be called after
// communication is done.
void disconnect(server_t *server){
	close(server->socket);
	free(server->buffer);
	free(server);
}
