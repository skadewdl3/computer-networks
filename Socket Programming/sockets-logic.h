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

typedef enum {
	StringRepsonse, IntegerResponse, DecimalResponse 
} ResponseType;

typedef struct sockaddr_in Address;

typedef struct {
	Address address;
	int socket_fd;
	int max_pending_connections;
} Socket;


typedef struct {
	int count;
	int max_count;
	fd_set* fds;
	Socket** sockets;
} Connections;

typedef struct {
	Address address;
	int socket_fd;
	Connections* connections;
	int max_pending_connections;
} MasterSocket;

typedef struct {
	char* data;
	int status;
} Response;

typedef void (*StopSocketListening)();
typedef void (*CloseConnection)();
typedef void (*SocketListener)(Socket* sock, Response* response, StopSocketListening stop, CloseConnection close);
 

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
	new_socket->max_pending_connections = 5;
	int enable = 1;
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEADDR");
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEPORT");
	return new_socket;
}

MasterSocket* create_master_socket (int socket_family, int socket_type, int protocol) {
	MasterSocket* new_socket = (MasterSocket*)malloc(sizeof(MasterSocket));
	new_socket->socket_fd = socket(socket_family, socket_type, protocol);
	new_socket->max_pending_connections = 5;
	if (new_socket->socket_fd < 0) error("\nError while creating socket");
	new_socket->connections = NULL;
	int enable = 1;
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEADDR");
	if (setsockopt(new_socket->socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) error("Error while setting option: SO_REUSEPORT");
	return new_socket;
}

Connections* create_connection_list (int max_count) {
	Connections* conn = (Connections*)malloc(sizeof(Connections));
	fd_set* fds = (fd_set*)malloc(sizeof(fd_set));
	conn->count = 0;
	conn->max_count = max_count;
	conn->fds = fds;
	conn->sockets = (Socket**)malloc(sizeof(Socket*) * conn->max_count);
	for (int i = 0; i < max_count; i++) {
		conn->sockets[i] = NULL;
	}
	return conn;
}

void destroy_connection_list (Connections* conn) {
	free(conn);
}

int update_connection_list (MasterSocket* sock) {
	int max_sd = sock->socket_fd;
	Connections* conn = sock->connections;
	FD_SET(sock->socket_fd, conn->fds);
	for (int i = 0; i < conn->max_count; i++) {
		Socket* sk = conn->sockets[i];
		if (sk != NULL) {
			FD_SET(sk->socket_fd, conn->fds);
			if (sk->socket_fd > max_sd) max_sd = sk->socket_fd;
		}
	}

	return max_sd;
}

void add_connection_list (MasterSocket* sock, Connections* conn) {
	sock->connections = conn;
}

void close_default_socket (Socket* sock) {
	close(sock->socket_fd);
	free(sock);
}

void close_master_socket (MasterSocket* sock) {
	close(sock->socket_fd);
	free(sock->connections);
	free(sock);
}

void add_address_default (Socket* sock, int port) {
	sock->address.sin_family = AF_INET;
	sock->address.sin_addr.s_addr = INADDR_ANY;
	sock->address.sin_port = htons(port);
}


void add_address_master (MasterSocket* sock, int port) {
	sock->address.sin_family = AF_INET;
	sock->address.sin_addr.s_addr = INADDR_ANY;
	sock->address.sin_port = htons(port);
}

void bind_socket_default (Socket* sock) {
	int bind_status = bind(sock->socket_fd, (struct sockaddr*)&sock->address, sizeof(sock->address));
	if (bind_status < 0) error("\nError while binding.");
}


void bind_socket_master (MasterSocket* sock) {
	int bind_status = bind(sock->socket_fd, (struct sockaddr*)&sock->address, sizeof(sock->address));
	if (bind_status < 0) error("\nError while binding.");
}

Socket* accept_connection_on_socket_default(Socket* sock) {
	Socket* new_socket = (Socket*)malloc(sizeof(Socket));
	socklen_t new_socklen = sizeof(new_socket->address);
	new_socket->socket_fd = accept(sock->socket_fd, (struct sockaddr*)&new_socket->address, &new_socklen);
	if (new_socket->socket_fd < 0) error("\nError while accepting");
	return new_socket;
}

Socket* accept_connection_on_socket_master(MasterSocket* sock) {
	Socket* new_socket = (Socket*)malloc(sizeof(Socket));
	socklen_t new_socklen = sizeof(new_socket->address);
	new_socket->socket_fd = accept(sock->socket_fd, (struct sockaddr*)&new_socket->address, &new_socklen);
	if (new_socket->socket_fd < 0) error("\nError while accepting");
	Connections* conn = sock->connections;
	for (int i = 0; i < conn->max_count; i++) {
		if (conn->sockets[i] == NULL) {
			conn->sockets[i] = new_socket;
			break;
		}
	}
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

Response* receive_on_socket_default(Socket* sock) {
	Response* response = (Response*)malloc(sizeof(Response));
	response->data = (char*)malloc(sizeof(char) * 1000);
	int receive_status = recv(sock->socket_fd, response->data, sizeof(char) * 1000, 0);
	if (receive_status < 0) {
		response->status = -1;
		free(response->data);
		error("\nError while receiving - connection closed before data was received.");
	}
	else if (receive_status == 0) {
		response->status = 0;
		free(response->data);
	}
	else {
		int length = strlen(response->data);
		response->data = realloc(response->data, (length + 1) * sizeof(char));
		response->data[length] = '\0';
		response->status = 1;
	}
	return response;
}

Response* receive_on_socket_master(MasterSocket* sock) {
	Response* response = (Response*)malloc(sizeof(Response));
	response->data = (char*)malloc(sizeof(char) * 1000);
	int receive_status = recv(sock->socket_fd, response->data, sizeof(char) * 1000, 0);
	if (receive_status < 0) {
		response->status = -1;
		free(response->data);
		error("\nError while receiving - connection closed before data was received.");
	}
	else if (receive_status == 0) {
		response->status = 0;
		free(response->data);
	}
	else {
		int length = strlen(response->data);
		response->data = realloc(response->data, (length + 1) * sizeof(char));
		response->data[length] = '\0';
		response->status = 1;
	}
	return response;
}

void listen_on_socket_default(Socket* sock) {
	if (listen(sock->socket_fd, sock->max_pending_connections) < 0) {
		error("\nError while listening.");
	}
}

void listen_on_socket_master (MasterSocket* sock, SocketListener callback) {
	if (listen(sock->socket_fd, sock->max_pending_connections) < 0) {
		error("\nError while listening.");
	}

	int should_stop_listening = 0;

	void stop_listening () {
		should_stop_listening = 1;
	}


	Connections* conn = sock->connections;
	fd_set* fds = conn->fds;	

	while (1) {

		if (should_stop_listening) break;

		int max_sd = update_connection_list(sock);
		int activity = select(max_sd + 1, fds, NULL, NULL, NULL);

		if (activity < 0) {
			printf("Error while selecting socket.");
		}

		if (FD_ISSET(sock->socket_fd, fds)) {
			accept_connection_on_socket_master(sock);
		}

		
		else {
			for (int i = 0; i < conn->max_count; i++) {
				if (conn->sockets[i] == NULL) continue;
				int sd = conn->sockets[i]->socket_fd;

				void close_connection () {
					close_default_socket(conn->sockets[i]);
					conn->sockets[i] = NULL;	
				}

				if (FD_ISSET(sd, fds)) {

					Response* response = receive_on_socket_default(conn->sockets[i]);
					if (response->status < 0) {
						close_connection();
					}
					else if (response->status == 0) {
						close_connection();
					}
					else {
						
						callback(conn->sockets[i], response, stop_listening, close_connection);
					}
				}
			}
		}
		
	}
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
