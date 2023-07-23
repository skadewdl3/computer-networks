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

#define INTERNET AF_INET
#define STREAM_SOCKET SOCK_STREAM
#define AUTO 0

typedef enum {
	StringRepsonse, IntegerResponse, DecimalResponse 
} ResponseType;

typedef struct sockaddr_in Address;

typedef struct {
	Address address;
	int socket_fd;
} Socket;

typedef struct {
	char* string;
	int* integer;
	float* decimal;
	ResponseType type;
} Response;
 
// A function to throw an error and exit the program
void error (char* message) {
	perror(message);
	exit(1);
}

// A function to throw an error and exit the program
void success (char* message) {
	printf("%s", message);
}	

Socket* create_socket (int socket_family, int socket_type, int protocol) {
	Socket* new_socket = (Socket*)malloc(sizeof(Socket));
	new_socket->socket_fd = socket(socket_family, socket_type, protocol);
	if (new_socket->socket_fd < 0) error("\nError while creating socket");
	int enable = 1;
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEADDR");
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEPORT");
	return new_socket;
}

void close_socket (Socket* sock) {
	close(sock->socket_fd);
	free(sock);
}

void add_address (Socket* socket, int port) {
	socket->address.sin_family = AF_INET;
	socket->address.sin_addr.s_addr = INADDR_ANY;
	socket->address.sin_port = htons(port);
}

void bind_socket (Socket* sock) {
	int bind_status = bind(sock->socket_fd, (struct sockaddr*)&sock->address, sizeof(sock->address));
	if (bind_status < 0) error("\nError while binding.");
}

void listen_on_socket(Socket* sock) {
	listen(sock->socket_fd, 5);
}

Socket* accept_connection_on_socket(Socket* sock) {
	Socket* new_socket = (Socket*)malloc(sizeof(Socket));
	socklen_t new_socklen = sizeof(new_socket->address);
	new_socket->socket_fd = accept(sock->socket_fd, (struct sockaddr*)&new_socket->address, &new_socklen);
	if (new_socket->socket_fd < 0) error("\nError while accepting");
	return new_socket;
}
 
void create_connection (Socket* sock) {
	int connection_status = connect(sock->socket_fd, (struct sockaddr*)&sock->address, sizeof(sock->address));
	if (connection_status < 0) error("\nError while connecting.");
}

void send_to_socket(Socket* sock, char* message) {
	send(sock->socket_fd, message, strlen(message), 0);
}

char* receive_on_socket(Socket* sock) {
	char* message = (char*)malloc(sizeof(char) * 1000);
	recv(sock->socket_fd, message, sizeof(char) * 1000, 0);
	int length = strlen(message);
	message = realloc(message, (length + 1) * sizeof(char));
	message[length] = '\0';
	return message;
}