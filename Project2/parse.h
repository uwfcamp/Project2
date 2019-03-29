//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//parse.h

#ifndef parse
#define parse
	
	void parse_message(char buffer[], int * mode, char username[], char password[], char destination[], char body[]);
	void clear_path(int inputPathArr[], int max_Size);
	void clear_string(char * string, int max_Size);

#endif
