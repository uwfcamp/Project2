//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//c_framework.c



// headers
#include "Definitions.h"
#include "c_framework.h"



// global control varibales
int force_close = 0;



int main(void) {
	char userInput[INPUT_SIZE];
	char body[INPUT_SIZE];
	int menuOption=-1;
	int server_socket;
	char ip[IP_LENGTH];
	ip[0]='\0';

	// Establish the connection with the server
	// *******************************************************************//
        //option to let user set up IP or use local network as default
        do {
		char input[INPUT_SIZE];
                printf("PRESS ENTER TO USE LOCAL NETWORK AS DEFAULT OR ENTER IP THAT YOU WANT TO COMMUNICATE WITH\n>> ");
		fflush(stdout);
                fgets(input, INPUT_SIZE, stdin);
                input[strlen(input)-1]=0;
                if(strlen(input)==0)//user just pressed enter
                        strcpy(ip, DEFAULT_IP);
                else if (strlen(input)<16 && strlen(input)>6)//valid IP case
                        strcpy(ip, input);
        }while(ip[0]=='\0');

	// create the socket for communication with the server
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr,"ERROR: socket creation error\n");
		return 0;
	}

	// structer for server address data
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	int address_length = sizeof(serv_addr);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
	{
		fprintf(stderr,"ERROR: invalid address and/or address not supported\n");
		return 0;
	}

	// connect to the server using the established socket
	if (connect(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "ERROR: connection failed\n");
		return 0;
	}
	//*******************************************************************//


	print_help();
	do {
		do{
			printf(">> ");
			fgets(userInput, INPUT_SIZE, stdin);
		} while(strlen(userInput) <= 1);
		menuOption=get_menu_option(userInput, body);
		switch(menuOption) {
			case 0: // quit case
				force_close=1;
				break;
			case 1: // ls case
				{
					char buffer[BUFFER_SIZE]={0};
					strcpy(buffer, "ls");

					// send the message
					errno = 0;
					send(server_socket, buffer, strlen(buffer)+1, MSG_NOSIGNAL);
					if (errno==EPIPE)
						force_close=1;
					int i;
					for(i=strlen(buffer);i>-1;i--)
						buffer[i]=0;

					// recieve back the string listing files
					int size = recv(server_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL);
					if (size==0)
						force_close=1;

					// print the recieved message
					if (force_close==0)
						printf("%s\n",buffer);
					else
						printf("YOUR CONNECTION WITH THE SERVER HAS BEEN BROKEN\n");
					break;
				}
			case 2: // put case
				{
					char buffer[BUFFER_SIZE]={0};
					char fileSize_str[BUFFER_SIZE]={0};
					long long fileSize_num=get_file_size(body);
					if (fileSize_num>=0){
						// send initial message to server
						strcpy(buffer, "put ");
						strcat(buffer, body);
						strcat(buffer, " ");
						sprintf(fileSize_str, "%lld", fileSize_num);
						strcat(buffer, fileSize_str);

						// ensure that the file can be opened before sending data to the server
						FILE *fp = fopen(body, "rb");
						if (fp!=NULL){
							send(server_socket, buffer, strlen(buffer)+1, MSG_NOSIGNAL);
							if (errno==EPIPE){
								printf("ERROR: CONNECTION BROKEN\n");
								force_close=1;
							}

							// clear out the buffer
							int i;
							for(i=strlen(buffer);i>-1;i--)
								buffer[i]=0;

							if (force_close!=1){
								while(fileSize_num>0){
									// read in up to BUFFER_SIZE bytes
									size_t bytes_read = fread(buffer,1,BUFFER_SIZE,fp);
									// decrement the file size tracker
									fileSize_num-=bytes_read;
									// send the buffered content
									send(server_socket,buffer, bytes_read, MSG_NOSIGNAL);
									// check if sent
									if (errno==EPIPE){
										printf("ERROR: CONNECTION BROKEN\n");
										force_close=1;
										break;
									}
								}
							}
							fclose(fp);
						}
					}
					else{
						printf("ERROR: FILE NOT FOUND\n");
					}
					break;
				}
			case 3: // get case
				{
					char buffer[BUFFER_SIZE]={0};
					long long filesize_num;
					sprintf(buffer, "get %s", body);
					send(server_socket, buffer, strlen(buffer)+1, MSG_NOSIGNAL);
					if (errno==EPIPE){
						printf("ERROR: CONNECTION BROKEN\n");
						force_close=1;
					}

					int bytes_recieved = recv(server_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL);
					if (bytes_recieved==0){
						printf("ERROR: CONNECTION BROKEN\n");
						force_close=1;
					}

					char *command;
					char *filename;
					char *filesize_str;
					if (force_close==0){
						command = strtok(buffer, "\n ");
						if (strcmp(command,"get")==0){
							filename = strtok(NULL, "\n ");
							filesize_str = strtok(NULL, "\n ");
							filesize_num = atoll(filesize_str);
							FILE *fp = fopen(filename,"wb");
							if (fp != NULL){
								while(filesize_num>0){
									int i;
									for (i=0; i<BUFFER_SIZE; i++)
										buffer[i]=0;
									// read data from server into buffer
									bytes_recieved = recv(server_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL);
									// check if connection is still valid
									if (bytes_recieved==0){
										printf("ERROR: CONNECTION BROKEN\n");
										force_close=1;
										break;
									}
									else if (bytes_recieved>0){
									// decrement the file size tracker
										filesize_num-=bytes_recieved;
									// write data recieved to file
										fwrite(buffer, 1, bytes_recieved, fp);
									}
								}
								fclose(fp);
							}
							else{
								printf("ERROR: COULD NOT WRITE TO FILE\n");
							}
						}
						else if (strcmp(command, "nf")){
							printf("ERROR: FILE NOT FOUND\n");
						}
						else{
							printf("ERROR: UNKNOWN ERROR ENCOUNTERED\n");
						}
					}
					break;
				}
			case 4: // pwd case
				//printf("pwd case\n"); // delete later
				{
					char buffer[BUFFER_SIZE]={0};
					strcpy(buffer, "pwd");

					// send the message
					errno = 0;
					send(server_socket, buffer, strlen(buffer)+1, MSG_NOSIGNAL);
					int i;
					for(i=strlen(buffer);i>-1;i--)
						buffer[i]=0;
					if (errno==EPIPE)
						force_close=1;

					// recieve back the string listing files
					int size = recv(server_socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL);
					if (size==0)
						force_close=1;

					// print the recieved message
					if (force_close==0)
						printf("%s\n",buffer);
					else
						printf("YOUR CONNECTION WITH THE SERVER HAS BEEN BROKEN\n");
					break;
				}
			case 5: // help case
				print_help();
				break;
			case 6: // echo case
				{
					char buffer[BUFFER_SIZE];
					buffer[0]=0;
					strcpy(buffer, "echo ");
					strcat(buffer, body);

					// send the message
					errno = 0;
					send(server_socket, buffer, strlen(buffer)+1, MSG_NOSIGNAL);
					if (errno==EPIPE)
						force_close=1;
					int i;
					for(i=strlen(buffer);i>-1;i--)
						buffer[i]=0;
					// recieve the same message back
					int size = recv(server_socket, buffer, INPUT_SIZE * 2, MSG_NOSIGNAL);
					if (size==0)
						force_close=1;

					// print the recieved message
					if (force_close==0)
						printf("%s\n",buffer);
					else
						printf("YOUR CONNECTION WITH THE SERVER HAS BEEN BROKEN\n");
					break;
				}
			case -2: // error case for get
				printf("Correct syntax for get is: get <file>\n");
				break;
			case -3: // error case for put
				printf("Correct syntax for put is: put <file>\n");
				break;
			case -4: // error case for echo
				printf("Correct syntax for echo is: echo <message>\n");
				break;
			default: // error case
				printf("INVALID INPUT\n");
				break;
		}
	}while(menuOption != 0 && force_close==0);



	close(server_socket);
	return 0;
}

int get_menu_option(char * userInput, char * body) {
	char *  search = "\n ";
	char * token;
	int i;

	// seperate command from parameters
	token = strtok(userInput, search);

	// convert all characters of the command to lower case
	for(i=0;i<strlen(token);i++)
		token[i]=tolower(token[i]);

	// compare the command string to the predefined commands
	if(strcmp(token ,"quit")==0)
		return 0;
	else if (strcmp(token, "ls")==0)
		return 1;
	else if (strcmp(token, "put")==0) {
		if((token = strtok(NULL, search)) != NULL) {
			strcpy(body, token);
			return 2;
		}
		else //invalid syntax for put
			return -3;
	}
	else if (strcmp(token, "get")==0){
		if((token = strtok(NULL, search)) != NULL) {
			strcpy(body, token);
			return 3;
		}
		else //invalid syntax for get
			return -2;
	}
	else if (strcmp(token, "pwd")==0)
		return 4;
	else if (strcmp(token, "help")==0)
		return 5;
	else if (strcmp(token, "echo")==0){
		if((token = strtok(NULL, search)) != NULL) {
			strcpy(body, token);
			return 6;
		}
		else{
			return -4;
		}
	}
	return -1; // no defined case matches
}



void print_help(void) {
	printf("-=| Client Commands |=-\nhelp: Display all commands for the server\nquit: Quit the server\nls: Display all files on the ftp server\nget <file>: Download file from the ftp server\nput <file>: Upload file to the ftp server\npwd: display the current path\necho <message>: Have the server echo a message back\n\n");
	return;
}



long long get_file_size(char *filename){
	struct stat st;
	if (stat(filename, &st) == 0)
		return st.st_size;
	return -1;
}
