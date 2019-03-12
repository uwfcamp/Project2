/*
 * Server side C program that echos
 * messages back to the client program
 *
 * @AUTHOR	Curtis Duvall, Nathan Wilkins, Julian Thayer
 * @DATE	3/8/2019
 * @INFO	COP4635 Project 1
 */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define PORT 60001
#define BUFFER_SIZE 30000

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    memset(&address, '\0', sizeof address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Creating socket file descriptor
    printf("STATUS: creating socket file descriptor\n");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    printf("STATUS: binding socket to address\n");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    printf("STATUS: setting server to listen for new connections\n");
    if (listen(server_fd, 20) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // This needs to be performed in a seperate thread,
    // so that new connections can be added, while
    // still listening to existing connections.
    printf("STATUS: waiting for connection request\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }

    // make it so that future calls to accept
    // new sockets do not block. This means
    // that the server will keep going if
    // there is no new client to connect to.
    int temp_socket;
    int flags = fcntl(server_fd, F_GETFL);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    while(1)
    {
	int err;

        // Here we will check to see if there is
        // a new connection request, and if there
        // is, we will need to add it to a list of
        // connections. If there is no new connection,
        // indicated by "errno", we just keep going.
	temp_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        err = errno;
        if (err !=EAGAIN || err != EWOULDBLOCK){
            // This is where we would add the
            // new socket to our list of sockets
            // XXX XXX XXX XXX
            // XXX XXX XXX XXX
            // XXX XXX XXX XXX
            // XXX XXX XXX XXX
        }

        // buffer for message acceptance
        char buffer[BUFFER_SIZE] = {0};

        // accept message from client, will
        // need to loop through all connections.
        // Would likely be more efficient to
        // have a seperate thread for each connection.
        //printf("STATUS: waiting for message\n");
        valread = recv( new_socket , buffer, BUFFER_SIZE, MSG_NOSIGNAL | MSG_DONTWAIT);
        err = errno;
        if (valread>0)
                printf("MESSAGE: %s\n", buffer );

        // determine where message needs to be sent
        // XXX XXX XXX XXX
        // XXX XXX XXX XXX
        // XXX XXX XXX XXX
        // XXX XXX XXX XXX

        // send the message to all the proper recipients
        if (valread>0){
                printf("STATUS: sending response message\n");
                valread = send(new_socket , buffer , valread, MSG_NOSIGNAL);
        }
    }
    close(temp_socket);
    close(new_socket);
    close(server_fd);
    printf("STATUS: connection broken\n");
    return 0;
}
