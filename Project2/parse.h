//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
//parse.h

#ifndef parse
#define parse
	
	// will parse the message into seperate data fields
	void parse_message(char buffer[], int * mode, char username[], char password[], char destination[], char body[]);
	// will clear the contents of a string
	void clear_string(char * string, int max_Size);

#endif
