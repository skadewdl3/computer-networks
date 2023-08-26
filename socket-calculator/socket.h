#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTO 0
#define DEFAULT_BUFFER_SIZE 1000
typedef struct sockaddr_in Address;

void check (int condition, char* message) {
	if (condition) printf("\n%s\n", message);
}

int create_socket (int family, int type, int protocol) {
	int socket_fd = socket(family, type, protocol);
	check(socket_fd < 0, "Error in creating socket");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nSocket created.");
	#endif
	return socket_fd;
}

Address* create_address (int family, int port) {
	Address* addr = (Address*)malloc(sizeof(Address));
	addr->sin_family = family;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = INADDR_ANY;
	return addr;
}

void set_socket_options (int socket_fd) {
	int enable = 1;
	check(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable)) < 0, "Error in setting socket options.");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nSocket options set.");
	#endif
}

void bind_socket (int socket_fd, Address* addr) {
	socklen_t addr_length = sizeof(*addr);
	check(bind(socket_fd, (struct sockaddr*)addr, addr_length) < 0, "Error in binding");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nBound to address.");
	#endif
}

void listen_on_socket (int socket_fd, int max_connections) {
	check(listen(socket_fd, max_connections) < 0, "Error in listening.");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nlistening");
	#endif
}

int accept_connection (int socket_fd, Address* addr) {
	socklen_t addr_length = sizeof(*addr);
	int connect_fd = accept(socket_fd, (struct sockaddr*)addr, &addr_length);
	check(connect_fd < 0, "Error in accepting connection");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nconnection accepted");
	#endif
	return connect_fd;
}

char* receive_from_socket (int socket_fd) {
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nreceiving");
	#endif
	char* buffer = (char*)malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
	int bytes_recvd = recv(socket_fd, buffer, DEFAULT_BUFFER_SIZE, 0);
	check(bytes_recvd < 0, "Error in receiving");
	check(bytes_recvd == 0, "Connection closed before data was received.");
	return buffer;
}

void send_on_socket(int socket_fd, char* data) {
	check(send(socket_fd, data, strlen(data), 0) < 0, "Error in sending.");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("\nData sent.");
	#endif
}

void close_socket (int socket_fd) {
	close(socket_fd);
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("Socket closed.");
	#endif
}

void connect_to_address (int socket_fd, Address* addr) {
	socklen_t addr_length = sizeof(*addr);
	check(connect(socket_fd, (struct sockaddr*)addr, addr_length) < 0, "Error in connecting");
	#ifndef SUPPRESS_SOCKET_LOGS 
	printf("Connection established.");
	#endif
}
