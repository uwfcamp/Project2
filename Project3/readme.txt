//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 3
//readme.txt

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\CONTENTS\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
\\  I.    -   SETUP AND COMPILATION                                                          \\
\\  II.   -   RUNNING THE PROGRAM                                                            \\
\\  III.  -   USER INPUT                                                                     \\
\\  IV.   -   USER OUTPUT                                                                    \\
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

I. SETUP AND COMPILATION

  1.  Download and unzip the submission folder from eLearning onto a Linux testing environtment
  
  2.  This sumbission should contain the following:
    - client_framework.c
    - client_framework.h
    - server_framework.c
    - server_framework.h
    - makefile
    - Definitions.h
    - readme.txt (this file)
    
  3.  Environment: This program was developed and tested in Linux. It was also tested in the
      multiplatform environment.
      
  4.  Compiling: This program contains a makefile. At the command line in Linux omitting the
      quotes("), type the command "make". This command will produce two executable files named
      "server" and "client". "server" is an executable that runs the server side of the project 
      description and "client" is an executable that runs the client side of the project 
      description. After successful compilation. Put the client executable in a separate 
      directory from the server(This will be mentioned a few times to make sure this is done).


II. RUNNING THE PROGRAM

  1.  server: Start the server program by entering "./server". No extra files are neccesary to 
      run this program. 
      
  2.  client: Put the client executable in a separate directory from the server. Start the client
      program by entering "./client". No extra files are neccesary to run this program.
      
III. USER INPUT

  1.  server: On startup the user will need to enter the IP address that it wants to connect
      through or hit enter to use the local default. After that, the user can input "help",
      "quit", "count current" or "count all"
      
      help: Displays a list of all commands available to the user.
      quit: safely exits the program.
      count current: Displays the count of all active visitors to the server program.
      count all: Displays the count of all visitors to the server program.
  
  2.  client: On startup the user will need to enter the IP address that it wants to connect
      to or hit enter to use the local default. After that, the user can input "help", "quit",
      "ls", "get <file>", "put <file>", "pwd", and "echo <message>"
      
      help: Displays a list of all commands available to the user.
      quit: safely exits the program.
      ls: Displays all files in the current working directory.
      get <file>: Dowload file from FTP server.
      put <file>: Upload file to FTP server.
      pwd: Display the present working directory.
      echo <message>: Have the server echo a message back.
  
IV. OUTPUT

  1.  server: Based upon the users input, the progam will display the apropriate output as outlined
      in the project description. The program will also output the connection making status.
  
  2.  client: Based upon the users input, the program will display the apropriate output as outlined
      in the project description. 
  
