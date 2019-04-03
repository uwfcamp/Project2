//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//s_framework.c

#include "Definitions.h"
#include "s_framework.h"
int main(int argc, char **argv) {

	//startup

	//option to let user set up IP or use local network as default

	//When a client connects, a new thread is created to handle each new connection.

	//shutdown

	return 0;
}
void print_menu(void) {
	printf("-=| Server Menu |=-\n");
	printf("help: Display all commands for ther server.\n");
	printf("quit: Quit the server.\n");
	printf("count current: Display number of currently active users.\n");
	printf("count all: Display the total number of system visitors.\n");
	return;
}
