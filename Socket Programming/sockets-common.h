// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Contains definitions of data types used in system calls.
// FOllowing two includes make use of these data types.
#include <sys/types.h>

// COntains definitions of structures and functions needed for
// socket programming
#include <sys/socket.h>

// Contains definitions of structures and functions needed for
// internet domain access
#include <netinet/in.h> 

// Contains the close function which closes a
// file descriptor and for forking process
 #include <unistd.h>

// Contains definitions of structures and functions for
// managing file descriptors (FD_SET, FD_ISSET, etc.)
#include <sys/time.h>

#define INTERNET AF_INET
#define STREAM_SOCKET SOCK_STREAM
#define AUTO 0
#define DEFAULT_BUFFER_LENGTH 1000


typedef struct sockaddr_in Address;

// A function to throw an error and exit the program
void error (char* message) {
    perror(message);
    exit(1);
}

