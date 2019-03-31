//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//Definitions.h


#ifndef Definitions
#define Definitions
	#include <stdio.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <netinet/in.h>
	#include <string.h>
	#include <fcntl.h>
	#include <time.h>
	#include <arpa/inet.h>
	#include <pthread.h>
	#include <stdlib.h>
	#include <semaphore.h>
	#include <sys/stat.h>
	#include <sys/time.h>

	//Global Definitions
	#define PORT 60001
	#define BUFFER_SIZE 30000
	#define CREDENTIAL_SIZE 20
	#define DELIMITER 176
	#define MAX_TIME_SIZE 50
	#define TIMEOUT_INTERVAL 15

	typedef struct server_s{
	int socket;		// identifier for the server socket
	char username[CREDENTIAL_SIZE];	// store the username of the user
	char password[CREDENTIAL_SIZE];	// store the password of the user
	char *buffer_in;	// Pointer to the buffer of stuff coming in from the server
	char *buffer_out;	// Pointer to the buffer of stuff going out to the server
	int buffer_size;	// The max size of buffer
	int buffered_in_size;	// The number of bytes in the input buffer
	int buffered_out_size;	// The number of bytes in the output buffer
	int send;		// 0 if there is nothing to be sent, 1 if there is
	int recieve;		// 0 if nothing has been recieved, 1 if something has
	int connected;		// 0 if not connected, 1 if connected
	int logged_in;		// 0 if not logged in, 1 if logged in
	int in_group_chat;
	int is_admin;
	int in_private_chat;
	int valid_destination;
	int is_banned_or_kicked;
	sem_t mutex;
	pthread_mutex_t lock;
	char username_private_chat[CREDENTIAL_SIZE];
	struct timeval *last_reception;
	}server_t;
	
	typedef struct client_list_s{
	int logged_in;			// 0 if the client is not logged in, 1 otherwise
	char username[CREDENTIAL_SIZE];	// the client's username
	char password[CREDENTIAL_SIZE];	// the client's password
	char is_banned[CREDENTIAL_SIZE];// is the client banned?
	int socket;			// identifier for the server socket
	int connected;			// 0 if not connected, 1 if connected
	struct client_list_s *last;	// pointer to the previous element in the list
	struct client_list_s *next;	// pointer to the next element in the list
	struct timeval *last_reception;
	int ping;
	}client_list_t;
	
	typedef struct admin_account_s {
		char username[CREDENTIAL_SIZE]; // admin username
		char password[CREDENTIAL_SIZE]; // admin password
	}admin_account_t;

	
#endif
