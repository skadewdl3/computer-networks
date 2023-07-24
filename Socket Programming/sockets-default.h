typedef struct {
	Address address;
	int socket_fd;
	int max_pending_connections;
} Socket;

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

void close_socket_default (Socket* sock) {
	close(sock->socket_fd);
	free(sock);
}

void add_address_default (Socket* sock, int port) {
	sock->address.sin_family = AF_INET;
	sock->address.sin_addr.s_addr = INADDR_ANY;
	sock->address.sin_port = htons(port);
}

void bind_socket_default (Socket* sock) {
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

void listen_on_socket_default(Socket* sock) {
	if (listen(sock->socket_fd, sock->max_pending_connections) < 0) {
		error("\nError while listening.");
	}
}


