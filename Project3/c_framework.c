//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//c_framework.c

#include "Definitions.h"
#include "c_framework.h"

int main(void) {
char userInput[INPUT_SIZE];
char body[INPUT_SIZE];
int menuOption=-1;

//establish connection, prompt if using different IP and get it otherwise user local default

do {
    do{
       printf(">> ");
       fgets(userInput, INPUT_SIZE, stdin);
    } while(strlen(userInput) <= 1);
    menuOption=get_menu_option(userInput, body);
    switch(menuOption) {
      case 0: // quit case
        printf("Quiting application\n"); // delete later
        break;
      case 1: // ls case
        printf("ls case\n"); // delete later
        break;
      case 2: // put case
        printf("put case\n"); // delete later
        break;
      case 3: // get case
        printf("get case\n"); // delete later
        break;
      case 4: // pwd case
        printf("pwd case\n"); // delete later
        break;
      case 5: // help case
        printf("help case\n"); // delete later
        print_help();
        break;
      default: // error case
        printf("error case\n"); // delete later
        printf("INVALID INPUT\n");
        break;
    }
}while(menuOption != 0) 


return 0;
}

int get_menu_option(char * userInput, char * body) {
    char search[3];
    char temp[INPUT_SIZE];
    char * token;
    int i;
    strcpy(temp, userInput);
    search[0]='\n';
    search[1]= ' ';
    search[3]='\0';
    if (token = strtok(temp, search)) != NULL) {
        for(i=0;i<strlen(token);i++) {
            token[i]=tolower(token[i]); 
        }
        if(strcmp(token ,"quit"))
           return 0;
        else if (strcmp(token, "ls"))
           return 1;
        else if (strcmp(token, "put")) {
           if((token = strtok(NULL, search)) != NULL) {
                strcpy(body, token);
                return 2;
           }
           else //invalid syntax for put
               return -1;
        }
        else if (strcmp(token, "get")){
           if((token = strtok(NULL, search)) != NULL) {
                strcpy(body, token);
                return 3;
           }
           else //invalid syntax for get
               return -1;
        }
        else if (strcmp(token, "pwd"))
           return 4;
        else if (strcmp(token, "help"))
           return 5;
    }
    return -1; // no defined case matches
}
void print_help(void) {
    printf("-=| Client Commands |=-\nhelp: Display all commands for the server\nquit: Quit the server\nls: Display all files on the ftp server\nget <file>: Download file from the ftp server\nput <file>: Upload file to the ftp server\npwd: display the current path\n\n");
    return;
}
