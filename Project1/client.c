/*
 * This program will allow the user
 * to make HTTP GET requests to a
 * server, given an IP address, and
 * the port that the server is
 * listening on.
 * 
 * @AUTHOR	Curtis Duvall, Nathan Wilkins
 * @DATE	2/21/2019
 * @INFO	COP4635 - Project 1
 */



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>



#define INPUT_SIZE 32
#define BUFFER_SIZE 1024



int main(void)
{
	// The integer for holding the socket identifier
	int client_socket = 0;
	long buffered_size;

	// The string for holding user input strings
	char input[INPUT_SIZE]={0};

	// holders for ip address and port number
	char *portS, *ip_address;
	int port;

	// user continue selection
	char action='y';

	// format identifier for server address
	struct sockaddr_in server_address;

	// this loop will allow the user to choose a new server to connect to
	while((action == 'y' || action == 'Y') && action != 'n' && action != 'N'){

		// Ask for the ip_address:port, and convert the
		// values and convert them to the proper format.
		// keep doing so until a valid input is given.
		int i = 1;
		do{
			if (i<=0)
			fprintf(stderr, "ERROR: Invalid address and/or Address not supported \n");

			printf("Please designate the socket you wish to connect to (127.0.0.1:60023): ");
			scanf(" %s", input);
			while(getchar()!='\n');
			ip_address = strtok(input," :\r\n");
			portS=strtok(NULL," :\r\n");
			if (portS!=NULL)
				port=atoi(portS);
			else
				port=80;

			//Set the address parameters for the server
			server_address.sin_family = AF_INET;
			server_address.sin_port = htons(port);
			server_address.sin_addr.s_addr = INADDR_ANY;
		}while((i = inet_pton(AF_INET, ip_address, &server_address.sin_addr))<=0);

		// This loop will allow the user to continually make requests for files
		while((action == 'y' || action == 'Y') && action != 'n' && action != 'N'){

			char *delim = " :\r\n";
			char filename[BUFFER_SIZE]={0};
			char request[BUFFER_SIZE]={0};
			char buffer[BUFFER_SIZE]={0};
			char *temp=NULL;
			long body_size=0;

			// create the client side socket
			printf("STATUS: creating client socket\n");
			if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				fprintf(stderr, "ERROR: Could not Create Socket\n");
				break;
			}

			// connect to the server
			printf("STATUS: connecting to the server");
			if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
			{
				printf("STATUS: could not connect to the server\n");
				close(client_socket);
				break;
			}

			// Prompt the user for file name
			filename[0]='\0';
			printf("File to request \"/FILENAME.EXT\": ");
			scanf("%s", filename);
			while(getchar()!='\n');

			// Construct HTTP request
			request[0]='\0';
			sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", filename, ip_address);
			printf("REQUEST:\n%s",request);

			// send the HTTP request to the server
			printf("STATUS: sending HTTP request\n");
			if (send(client_socket , request , strlen(request) , MSG_NOSIGNAL)<0){
				printf("STATUS: could not send request, connection broken\n");
				break;
			}

			// recieve initial HTTP response
			buffer[0]='\0';
			if ((buffered_size = recv( client_socket , buffer, BUFFER_SIZE, MSG_NOSIGNAL))<0){
				printf("STATUS: connection broken, cannot read more data\n");
				printf("%s\n",buffer);
			}
			printf("\n\n%s", buffer);

			// establish where the body of the response starts
			int body_start;
			for(body_start=0; body_start<buffered_size; body_start++){
				if (buffer[body_start]=='\n' && buffer[body_start+1]=='\n'){
					body_start+=2;
					break;
				}
				else if (buffer[body_start]=='\r' && buffer[body_start+1]=='\n' && buffer[body_start+2]=='\r' && buffer[body_start+3]=='\n'){
					body_start+=4;
					break;
				}
			}

			// tokenize the input, so that we can see
			// how big the body is supposed to be and
			// ensure the entire file has been delivered
			if ((temp=strtok(buffer,delim))!=NULL){
				do{
					if (strcmp(temp,"Content-Length")==0){
						temp = strtok(NULL, delim);
						body_size = atoi(temp);
					}
				}while((temp=strtok(NULL, delim))!=NULL && temp+strlen(temp)<&(buffer[body_start]));

				// count how much of the body has been recieved
				body_size -= buffered_size-body_start;

				// print the rest of the message that didn't fit in the buffer
				if (body_size>0){
					char *secondary_buffer = malloc(body_size+1);
					if ((recv(client_socket, secondary_buffer, body_size, MSG_NOSIGNAL))<0){
						printf("STATUS: the connection with the client seems to have been interupted\n");
						close(client_socket);
						free(secondary_buffer);
						continue;
					}
					else{
						int i;
						for (i=0; i<body_size; i++){
							printf("%c",secondary_buffer[i]);
						}
					}
					free(secondary_buffer);
				}
			}
			printf("\n\n");

			// close out the socket
			close(client_socket);

			// ask the user if a new file is desired
			do{
				printf("\nDo you want another file? (y/n): ");
				scanf(" %c", &action);
				while(getchar()!='\n');
			}while(action!='y' && action!='Y' && action!='n' && action!='N');
		}

		// ask the user if they want to connect to a different server
		do{
			printf("\nDo you want to connect to another server? (y/n): ");
			scanf(" %c", &action);
			while(getchar()!='\n');
		}while(action!='y' && action!='Y' && action!='n' && action!='N');

	}
	return 0;
}
