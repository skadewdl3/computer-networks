// Standard libraries
#include <stdio.h>
#include <stdlib.h>

// Contains definitions of data types used in system calls.
// FOllowing two includes make use of these data types.
#include <sys/types.h>

// COntains definitions of structures and functions needed for
// socket programming
#include <sys/socket.h>

// Contains definitions of structures and functions needed for
// internet domain access
#include <netinet/in.h> 

// Contains the close function which closes a socket
 #include <unistd.h>

// A function to throw an error and exit the program
void error (char* message) {
	perror(message);
	exit(1);
}

// A function to throw an error and exit the program
void success (char* message) {
	printf("%s", message);
}

int main () {
	// Initialise some variables related to sockets
	// on which server and client run
	int port_no = 8081;


	/* The socket() function creates a socket that accepts three agruments.
	1. Address Domain - Whether the socket operates in UNIX domain (two processes on same system)
						or internet domain (two processes anywhere on the internet). Here, we have
						selected the AF_INET i.e. Internet domain. AF_UNIX is to be passed for
						two processes that share a filesystem.
	2. Socket Type - 	Sockets are of two types - stream and datagram sockets. Stream sockets
						read a continuous stream of characters whereas datagram sockets read
						the data in chunks. We use stream socket hence pass in SOCK_STREAM. If we
						use a datagram socket, we will need to pass SOCK_DGRAM.
	3. Protocol - 		If this argument is 0, OS will choose most appropriate protocol for the
						socket i.e. TCP for stream sockets and UDP for datagram sockets. This
						argument should only be changed in specialised application.

	
	The function returns an entry in the socket descriptor table. This is a small integer that will
	be used to reference this socket from now on.
	*/
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);


	/* Contains an internet address. Define in <netinet/in.h>
		struct sockaddr_in {
	        short   sin_family;
	        u_short sin_port;
	        struct  in_addr sin_addr;
	        char    sin_zero[8];
		};
		struct in_addr {
			unsigned long s_addr;
		}
	*/
	struct sockaddr_in server_address;

	// Family of the socket
	server_address.sin_family = AF_INET;
	// htons converts string IP address to network-byte order form
	server_address.sin_port = htons(port_no);
	// INADDR_ANY is a predefine constant that gets the IP address of the current machine 
	server_address.sin_addr.s_addr = INADDR_ANY;


	const int enable = 1;
if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    error("setsockopt(SO_REUSEADDR) failed");
if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
    error("setsockopt(SO_REUSEADDR) failed");

	/* The bind() function binds the created socket to the server_address struct. It takes 3
	arguments i.e. the socket file descriptor, the address to which it is bound and the size of
	the address to which it is bound.

	1. Socket file descriptor - (int), returned value from socket() function
	2. Bind Address - (struct sockaddr_in *), pointer to server_address struct
	3. Bind Address size - (unsigned int), sizeof(server_address)
	*/

	int bind_status = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	if (bind_status < 0) error("Binding Failed\n");
	else success("Binding successful\n");


	/* The listen() function makes the server start listening to requests (of the TCP family as we
	passed in 0 as 3rd argument to socket() function). The function takes two arguments - the
	server address and the maximum number of connections that can be waiting while the process is
	handling a connection.

	1. Socket Number	 - 			(int), used to specify which socket is listening, this is the
									value returned from the socket() function.
	2. Max. waiting connections - 	(int), maximum no. of connections that can be kept waiting.
									for most OSes, this value is 5.

	*/
	listen(server_socket, 5);
	success("Sending reply to client.");

	int client_socket = accept(server_socket, NULL, NULL);

	if (client_socket < 0) {
		error("\nConnection rejected by server.");
	}
	else {
		success("\nConnection to server established.");
	}


	char server_message[] = "Hello from the server";
	send(client_socket, server_message, sizeof(server_address), 0);
	close(server_socket);

	return 0;
}