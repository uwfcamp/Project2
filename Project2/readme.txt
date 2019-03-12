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
    - client.c
    - server.c
    - makefile
    - rabbit.jpg
    - turtle.jpg
    - salmon.jpg
    - eagle.jpg
    - running_project_1.jpg
    - running_project_2.jpg
    - running_project_3.jpg
    - index.html
    - 404.html
    - works.html
    - works_with_image.html
    - project_1_protocol.docx (Protocol Document)
    - readme.txt (this file)
    
  3.  Environment: This program was developed and tested in Linux. It was also tested in the
      multiplatform environment.
      
  4.  Compiling: This program contains a makefile. At the command line in Linux omitting the
      quotes("), type the command "make". This command will produce two executable files named
      "tcpS" and "tcpC". "tcpS" is an executable that runs the server side of the project 
      description and "tcpC" is an executable that runs the client side of the project 
      description.


II. RUNNING THE PROGRAM

  1.  tcpS:  This program does not require any extra files to run. To execute the server
      program, the command for execution looks like the following: "./tcpS" no other arguments
      or user input is required for execution of this program.
      
  2.  tcpC: This program does not require any extra files to run. To execute this
      the client, the command for execution looks like the following: "./tcpC" no other
      arguments are required to begin the program.
      
  3.  Browser:  The browser part of this program can be activated in the browser of your choosing
      by inputting the address and port "127.0.0.1:60023" in your address bar.

III. USER INPUT

  1.  tcpS:   This program requires no user input to execute. 
  
  2.  tcpC:   First the program will ask the user to input the address and port number for the 
      connection that the files will be transferred on. The address and port number are already
      built into the Server and should be entered as "127.0.0.1:60023". The user can then 
      call upon any of the files in the servers directory to be transferred over a tcp
      connection from the server to the client with the syntax "/filename.extension". After
      the transfer is complete the user will be prompted to input 'y' or 'n' if any more files
      are to be transferred. If new files are to be transferred the client will ask for the
      filename extension again and loop back to this part. If the user inputs 'n' the program
      will ask if another connection should be made. If the user inputs 'n' the program will
      execute.

  3.  Browser:   After the index file is pulled up through activating the browser part of the
      program, the user uses the mouse pointer to navigate through the different web pages.

IV. OUTPUT

  1.  tcpS: The server program steps the user through the process of creating the tcp connection
            and responses to requests from connected programs. Sample output can be seen below.
            

  2.  tcpC: The client program will prompt the user for the correct input then show the steps
      of creating socket and the file that the tcp Server responds back with.
      
