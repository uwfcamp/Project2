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

	// data structure on the client side, for communicating with the server
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
	int in_group_chat;	// 0 if not in group chat, 1 otherwise
	int is_admin;		// 0 if not administrator, 1 otherwise
	int in_private_chat;	// 0 if not in private chat, 1 otherwise
	int valid_destination;  // 0 if destination of message is not valid, 1 otherwise
	int is_banned_or_kicked;// -1 if user has been kicked out, 0 if not, and 1 if banned
	sem_t mutex;		// semaphore for waiting until messages are processed by the communication thread
	pthread_mutex_t lock;	// lock, to block messages from printing while typing
	char username_private_chat[CREDENTIAL_SIZE]; // username of the person your private chat message is for
	struct timeval *last_reception; // time stamp for the last time a message was recieved from the server
	}server_t;
	
	// link list data structure on the server side, for communicating with all the clients
	typedef struct client_list_s{
	int logged_in;			// 0 if the client is not logged in, 1 otherwise
	char username[CREDENTIAL_SIZE];	// the client's username
	char password[CREDENTIAL_SIZE];	// the client's password
	char is_banned[CREDENTIAL_SIZE];// is the client banned?
	int socket;			// identifier for the server socket
	int connected;			// 0 if not connected, 1 if connected
	struct client_list_s *last;	// pointer to the previous element in the list
	struct client_list_s *next;	// pointer to the next element in the list
	struct timeval *last_reception; // time stamp for the last time a message was recieved from the client
	int ping;			// 0 if a ping does not need to be sent, 1 if a ping does need to be sent
	}client_list_t;
	
	typedef struct admin_account_s {
		char username[CREDENTIAL_SIZE]; // admin username
		char password[CREDENTIAL_SIZE]; // admin password
	}admin_account_t;

	
#endif
