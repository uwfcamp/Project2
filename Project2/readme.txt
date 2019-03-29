Curtis Duvall, Nathan Wilkins, and Julian Thayer
Systems and Networking II
Project 2
readme.txt

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
      description.


II. RUNNING THE PROGRAM

  1.  server: Begin the server side by executing the command "./server" in the directory the
      server executable is located in.
      
  2.  client: Begin the client side by executing the command "./client" in the directory the
      client executable is located in.
      
III. USER INPUT

  1.  server: This process does not require any input files to run. Optionally the user can
      provide a logins.txt formatted appropriately with username, password, and a 0 for client
      not being banned or 1 for a client be banned separated by the ascii character that has
      the decimal value of 176. On startup of the program, the user will be prompted to enter
      a username and password for the admin account on the server.
  
  2.  client: This process does not require any input files to run. The program will take
      user input based on requirements in the sheet. In most cases, there is an exit case for
      loops of either the letter 'q' or the string "_q". The specific case required will be 
      shown in each loop.

IV. OUTPUT

  1.  server:

  2.  client:
  
