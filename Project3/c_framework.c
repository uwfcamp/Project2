//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//c_framework.c

#include "Definitions.h"
#include "c_framework.h"

int main(void) {
char userInput[INPUT_SIZE];
int menuOption=-1;

//establish connection, prompt if using different IP and get it otherwise user local default

do {
    do{
       printf(">> ");
       fgets(userInput, INPUT_SIZE, stdin);
    } while(strlen(userInput) <= 1);
    menuOption=get_menu_option(userInput);
    switch(menuOption) {
      case 0: // quit case
        break;
      case 1: // ls case
        break;
      case 2: // put case
        break;
      case 3: // get case
        break;
      case 4: // pwd case
        break;
      case 5: // help case
        break;
      default: // error case
        printf("INVALID INPUT\n");
        break;
    }
}while(menuOption != 0) 


return 0;
}

int get_menu_option(char * userInput) {
    char search[3];
    char temp[INPUT_SIZE];
    char * token;
    int i;
    strcpy(temp, userInput);
    search[0]='\n';
    search[1]= ' ';
    search[3]='\0';
    token = strtok(temp, search);
    for(i=0;i<strlen(token);i++) {
      token[i]=tolower(token[i]); 
    }
    if(strcmp(token ,"quit"))
       return 0;
    else if (strcmp(token, "ls"))
       return 1;
    else if (strcmp(token, "put"))
       return 2;
    else if (strcmp(token, "get"))
       return 3;
    else if (strcmp(token, "pwd"))
       return 4;
    else if (strcmp(token, "help"))
       return 5;
    return -1;
}
