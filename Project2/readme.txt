//Curtis Duvall, Nathan Wilkins, and Julian Thayer
//Systems and Networking II
//Project 2
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
    - c_login_funct.c
    - c_login_funct.h
    - c_menu_funct.c
    - c_menu_funct.h
    - server_framework.c
    - server_framework.h
    - s_login_funct.c
    - s_login_funct.h
    - s_menu_funct.c
    - s_menu_funct.h
    - makefile
    - parse.h
    - parse.c
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

  1.  server: Begin the server side by executing the command "./server" in the directory the
      server executable is located in.
      
  2.  client: Before execution make sure your client is in a different directory than the
      server is in.Begin the client side by executing the command "./client" in the directory
      the client executable is located in.
      
III. USER INPUT

  1.  server: This process does not require any input files to run. Optionally the user can
      provide a logins.txt formatted appropriately with username, password, and a 0 for client
      not being banned or 1 for a client be banned separated by the ascii character that has
      the decimal value of 176. On startup of the program, the user will be prompted to enter
      a username and password for the admin account on the server.
  
  2.  client: This process does not require any input files to run. The program will take
      user input based on requirements in the sheet. In most cases, there is an exit case for
      loops are either the letter 'q' or the string "_q". The specific case required will be 
      shown in each loop. For the file transfer to work, the client executable must be in a
      different directory than the server with the file to transfer residing in the clients
      directory.

IV. OUTPUT

  1.  server: Upon startup, the server will prompt the user to input the password and username
      for the server administrator. Then the server will print each transition, request and 
      ping to and from clients to the screen. Through execution of the program, a logins.txt,
      groupchat.txt, privatechat.txt, filelist.txt are created.
      
      a.  logins.txt: This files contains all of the usernames, passwords, and the details
          signifying which users are banned. These details are shown by line with each value
          separated by the ascii character with the decimal value of 176. for the isbanned
          detail. a 0 signifies the user is not banned and a 1 signifies that the user is
          banned.
          
      b.  groupchat.txt: This file is the repository for all group communication. It contains
          all of the group chat logs in a running chat log. With a timestamp followed by the
          user who the broadcast came from preceeding the message. The maximum size that this
          file can be for the program to function properly is the size of BUFFER_SIZE ascii
          characters as specified in Definitions.h
          
      c.  privatechat.txt
      
      d.  filelist.txt

  2.  client:
  
