#include "parse.h"
#include "Definitions.h"

void parse_numbers(int inputPathArr[], char buffer[], char messageIn[])
{
	char * search = " ";
	char * token;
	char temp_string[MAX_PATH_ID_LENGTH]={0};
	int i, j;
	clear_string(messageIn, BUFFER_SIZE);
	clear_path(inputPathArr, MAX_PATH_ID_LENGTH);
	//parse out path and identifiers into separate string
	for(i=0;(i<MAX_PATH_ID_LENGTH) && (buffer[i] != '\n');i++)
		temp_string[i]=buffer[i];
	//parse out message into messageIn
	for(i=i+1, j=0;i<MAX_PATH_ID_LENGTH && buffer[i] != '\n';i++, j++)
		messageIn[j]=buffer[i];
	i = 0;
	//convert path temp string into integer inputPathArr
	token = strtok(temp_string, search);
	while(token != NULL) {
		inputPathArr[i] = atoi(token);
		i++;
		token = strtok(NULL, search);
	}
	return;
}

void clear_path(int inputPathArr[], int max_Size) {
	int i;
	for(i=0;i<max_Size;i++)
		inputPathArr[i]=0;
	return;
}

void clear_string(char * string, int max_Size) {
	int i;
	for (i=0;i<max_Size;i++)
		string[i]=0;
	return;
}
