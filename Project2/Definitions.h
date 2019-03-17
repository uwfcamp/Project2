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
	#include <pthread.h>
	#include <sched.h>
	#include <arpa/inet.h>
	#include <semaphore.h>
	
	//Global Definitions
	#define PORT 60001
	#define BUFFER_SIZE 30000
	#define CREDENTIAL_SIZE 20
	#define DELIMITER 176
	#define MAX_TIME_SIZE 50

#endif
