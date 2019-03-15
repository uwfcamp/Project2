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
	
	//Global Definitions
	#define PORT 60001
	#define BUFFER_SIZE 30000
	#define MAX_PATH_ID_LENGTH 20
	#define DELIMITER 176
	#define HEADER_DELIMITER 177

#endif
