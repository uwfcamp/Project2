/*
 * This program acts as a server, listening in on the port
 * designated in the preprocesser definition PORT.
 * Once the server has set up the port to listen for incoming
 * traffic, it will continue to listen, until the server process
 * is forcefully terminated.
 *
 * @AUTHOR	Curtis Duvall, Nathan Wilkins
 * @DATE	2/21/2019
 * @INFO	COP4635 Project 1
 */



#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>



#define BUFFER_SIZE 4096
#define HEADER_SIZE 1024
#define NAME_SIZE 512
#define TYPE_SIZE 64
#define PORT 60023



/*
 * This function will give the size of
 * a file, so it can be included in the
 * header of the HTTP response.
 *
 * char *filename - the name of the file,
 * relative to the server application's
 * location.
 *
 * return int - the size, in bytes, of
 * the file.
 */
int file_size(char *filename);



/*
 * This function will set all the
 * characters in a string to lowercase,
 * so that no matter how the file was
 * given in the URL, it can be retrieved.
 *
 * char *str - the filename to adjust to
 * all lower case.
 */
void str_lower(char *str);



int main(void){
	// declare integers to contain socket identifier's
	int server_socket, new_socket;

	// create the socket for listening for incoming traffic
	printf("STATUS: creating socket\n");
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0))<0){
		//if the socket could not be created, end the process
		fprintf(stderr, "ERROR: could not create socket\n");
		return 0;
	}

	// create the address for the listening socket
	printf("STATUS: creating address\n");
	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(PORT);
	socket_address.sin_addr.s_addr = INADDR_ANY;
	int address_length = sizeof(socket_address);

	// bind the socket to the address
	printf("STATUS: binding socket to address\n");
	if (bind(server_socket,(struct sockaddr *)&socket_address,sizeof(socket_address))<0){
		//if the socket cannot be bound to the address, end the process
		fprintf(stderr, "ERROR: could not bind socket to address\n");
		return 0;
	}

	// set the process to listen on the socket for incoming data
	printf("STATUS: setting process to listen to socket\n");
	if (listen(server_socket,5)<0){
		// if the process is unable to listen to the socket, end the process
		fprintf(stderr, "ERROR: could not listen to socket\n");
		return 0;
	}

	// continually listen for incoming http requests
	// until the user forces the program to end.
	while(1)
	{
		printf("STATUS: beginning server listening process\n");
		//listen on the port, so long as we have not recieved valid data
		while((new_socket=accept(server_socket,(struct sockaddr*)&socket_address,(socklen_t*)&address_length))<0){
			// there was a problem with the data given
			fprintf(stderr, "ERROR: socket could not accept new connection request\n");
		}
		
		printf("STATUS: connection to client established\n");

		// intermediary data holders
		char *delim = " :\r\n";
		char file_type[TYPE_SIZE]={0};
		char status_type[TYPE_SIZE]={0};
		char buffer[BUFFER_SIZE]={0};
		char filename[NAME_SIZE]={0};
		char header[HEADER_SIZE]={0};
		char *file_extension=NULL;
		char *temp=NULL;
		int status_code;
		int body_size;
		int valid_command = 0;
		long buffered_size=0;
		long request_body_size=0;

		// Read the incoming data into the buffer
		// and if their is a problem, simply go back
		// to listening for more requests
		printf("STATUS: awaiting incoming message from client\n");
		if ((buffered_size=recv(new_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL))<0){
			printf("STATUS: the connection with the client seems to have been interupted\n");
			close(new_socket);
			continue;
		}

		printf("STATUS: %ld byte initial client message recieved\n\n\n*********************************\n%s*********************************\n\n\n", buffered_size, buffer);

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

		// Tokenize the data into individual components, so they can be used
		printf("STATUS: tokenizing recieved message\n");
		if ((temp=strtok(buffer,delim))!=NULL){
			do{
				//look for the GET command
				if(strcmp(temp,"GET")==0){
					valid_command=1;
					temp = strtok(NULL, delim);
					strcpy(filename, temp);
					int i;

					//truncate the filename to all lower case
					str_lower(filename);

					//identify the extension of the file
					for (i=0; filename[i]!='\0'; i++)
						if (filename[i]=='.')
							file_extension=&(filename[i+1]);
				}
				//look for the body length descriptor
				if (strcmp(temp,"Content-Length")==0){
					temp = strtok(NULL, delim);
					request_body_size = atoi(temp);
				}
			}while((temp=strtok(NULL, delim))!=NULL);

			// count how much of the body has been recieved
			request_body_size -= buffered_size-body_start;

			// print the rest of the message that didn't fit in the buffer
			if (request_body_size>0){
				char *secondary_buffer = malloc(request_body_size+1);
				if ((recv(new_socket, secondary_buffer, request_body_size, MSG_NOSIGNAL))<0){
					printf("STATUS: the connection with the client seems to have been interupted\n");
					close(new_socket);
					free(secondary_buffer);
					continue;
				}
				else{
					int i;
					for (i=0; i<body_size; i++)
						printf("%c",secondary_buffer[i]);
				}
				free(secondary_buffer);
			}
		}
		printf("STATUS: message recieved from client\n");

		// check the formatting of the filename
		if (filename[1]==0 || filename[0]==0){
			// no file name was given, default to index.html
			strcpy(filename, "/index.html");
			file_extension = &filename[7];
		}
		else if (file_extension==NULL){
			// no extension was given with the filename, default to HTML
			int i =strlen(filename);
			strcat(filename,".html");
			file_extension=&filename[i+1];
		}
		else if (strcmp(file_extension, "htm")==0){
			// the extension was given as htm, truncate to html
			strcat(filename,"l");
		}


		// Figure out the status of the message, and the size of the file
		printf("STATUS: establishing response status code and size\n");
		if (!valid_command){
			// The server is forbidden from fulfilling the requested command
			printf("STATUS: the requested command is forbidden, sending 403 page\n");
			status_code=403;
			strcpy(status_type, "Forbidden");

			if ((body_size=file_size("403.html"))<0)
				body_size=0; //in case no 404 file is provided
			strcpy(filename,"/403.html");
			file_extension = &filename[5];
		}
		else if ((body_size = file_size(&filename[1]))<0){
			// The file was not found, send 404 page
			printf("STATUS: the requested file was not found, sending 404 page\n");
			status_code=404;
			strcpy(status_type, "Not Found");

			if ((body_size=file_size("404.html"))<0)
				body_size=0; //in case no 404 file is provided
			strcpy(filename,"/404.html");
			file_extension = &filename[5];
		}
		else{
			// The file was found, send 200 OK with file
			status_code=200;
			strcpy(status_type, "OK");
			printf("STATUS: file %s is %d bytes\n", &filename[1], body_size);
		}

		//identify the file type based off the extension
		printf("STATUS: establishing file type\n");
		if (strcmp(file_extension,"txt")==0 || strcmp(file_extension, "html")==0 || strcmp(file_extension, "css")==0 || strcmp(file_extension, "js")==0)
			strcpy(file_type,"text");
		else if (strcmp(file_extension,"gif")==0 || strcmp(file_extension, "png")==0 || strcmp(file_extension, "bmp")==0 || strcmp(file_extension, "jpg")==0 || strcmp(file_extension, "jpeg")==0 || strcmp(file_extension,"ico")==0)
			strcpy(file_type,"image");
		else if (strcmp(file_extension,"mp3")==0 || strcmp(file_extension,"wav")==0 || strcmp(file_extension, "wma")==0 || strcmp(file_extension,"ogg")==0)
			strcpy(file_type,"audio");
		else if (strcmp(file_extension,"mp4")==0 || strcmp(file_extension,"wmv")==0)
			strcpy(file_type,"video");
		else if (0) //no application file type set upt to work yet
			strcpy(file_type,"application");
		else
			strcpy(file_type,"text");

		// create the header for the outgoing file based on the data collected so far
		printf("STATUS: contstructing response header\n");
		sprintf(header,"HTTP/1.1 %d %s\r\nContent-Type: %s/%s\r\nContent-Length: %d\r\n\r\n", status_code, status_type, file_type, file_extension, body_size);

		// Send the header through the port
		printf("STATUS: sending header to client:\n\n\n*******************************************\n%s*******************************************\n\n\n",header);
		if (send(new_socket,header,strlen(header),MSG_NOSIGNAL)<0){
			printf("STATUS: connection has been broken, data could not be sent\n");
			close(new_socket);
			continue;
		}

		//add the file to the message body
		if (body_size>0){
			printf("STATUS: opening file for output stream\n");
			char *file_buffer = (char*)malloc(body_size);
			FILE *fp = fopen(&filename[1],"rb");

			if (fp!=NULL){
				printf("STATUS: reading file into the buffer\n");
				fread(file_buffer,1,body_size,fp);
				fclose(fp);

				printf("STATUS: sending buffer to client\n");
				if (send(new_socket,file_buffer,body_size,MSG_NOSIGNAL)<0)
					printf("STATUS: Connection has been broken, file could not be sent\n");
				else
					printf("STATUS: file has been sent to client, preparing for next request\n");
			}
			else{
				// The file could not be opened
				fprintf(stderr,"ERROR: file could not be opened\n");
			}

			// Free the memory used to load the file
			free(file_buffer);
		}

		// The file has been sent, close the secondary socket
		close(new_socket);
	}

	// The program is ending, close down the main socket
	close(server_socket);
	return 0;
}



// give the size of the requested file, -1 means it couldn't be found
int file_size(char *filename){
	int size=0;
	FILE *fp = fopen(filename,"rb");
	if (fp!=NULL){
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fclose(fp);
	}
	else{
		return -1;
	}
	return size;
}



// convert upercase characters in the string to lowercase
void str_lower(char *str){
	int i;

	for (i=0; str[i]!='\0'; i++)
		if (str[i]>='A' && str[i]<='Z')
			str[i]+='a'-'A';
}
