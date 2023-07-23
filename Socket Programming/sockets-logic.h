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
	Address address;
	int socket_fd;
	Socket** connections;
	int connection_count;
	int max_connections;
} MasterSocket;

typedef struct {
	char* data;
	int status;
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

MasterSocket* create_master_socket (int socket_family, int socket_type, int protocol, int max_connections) {
	MasterSocket* new_socket = (MasterSocket*)malloc(sizeof(MasterSocket));
	new_socket->socket_fd = socket(socket_family, socket_type, protocol);
	if (new_socket->socket_fd < 0) error("\nError while creating socket");
	new_socket->max_connections = max_connections;
	new_socket->connections = NULL;
	int connection_count = 0;
	int enable = 1;
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEADDR");
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEPORT");
	return new_socket;
}

void close_default_socket (Socket* sock) {
	close(sock->socket_fd);
	free(sock);
}

void close_master_socket (MasterSocket* sock) {
	printf("Closing master socket");
	close(sock->socket_fd);
	free(sock->connections);
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

void listen_on_socket(Socket* sock, int max_pending_connections) {
	if (listen(sock->socket_fd, max_pending_connections) < 0) {
		error("\nError while listening.");
	}
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

/*
int data = rand();
char* tosend = (char*)&data;
int remaining = sizeof(data);
int result = 0;
int sent = 0;
while (remaining > 0) {
    result = send(connfd, tosend+sent, remaining, 0);
    if (result > 0) {
        remaining -= result;
        sent += result;
    }
    else if (result < 0) {
        printf("ERROR!\n");
        // probably a good idea to close socket
        break;
    }
}
*/

void send_to_socket(Socket* sock, char* message) {
	send(sock->socket_fd, message, strlen(message), 0);
}

/*
int value = 0;
char* recv_buffer = (char*)&value;
int remaining = sizeof(int);
int received = 0
int result = 0;
while (remaining > 0) {
    result = recv(connfd, recv_buffer+received, remaining, 0);
    if (result > 0) {
        remaining -= result;
        received += result;
    }
    else if (result == 0) {
        printf("Remote side closed his end of the connection before all data was received\n");
        // probably a good idea to close socket
        break;
    }
    else if (result < 0) {
        printf("ERROR!\n");
        // probably a good idea to close socket
        break;
    }
}
*/

Response* receive_on_socket(Socket* sock) {
	Response* response = (Response*)malloc(sizeof(Response));
	response->data = (char*)malloc(sizeof(char) * 1000);
	int receive_status = recv(sock->socket_fd, response->data, sizeof(char) * 1000, 0);
	if (receive_status < 0) {
		response->status = -1;
		free(response->data);
		response->data = NULL;
		error("\nError while receiving - connection closed before data was received.");
		return response;
	}
	int length = strlen(response->data);
	response->data = realloc(response->data, (length + 1) * sizeof(char));
	response->data[length] = '\0';
	response->status = 1;
	return response;
}



char* parse_str (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as string - response is invalid.");
		return NULL;
	}
	return response->data;
}
int parse_int (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as int - response is invalid.");
		return -1;
	}
	return atoi(response->data);
}

double parse_float (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as float - response is invalid.");
		return -1;
	}
	return atof(response->data);
}
