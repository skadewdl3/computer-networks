#include "socket.h"

int main () {

	int server_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(server_fd);
	bind_socket(server_fd, server_address);
	listen_on_socket(server_fd, 5);
	int client_fd = accept_connection(server_fd, server_address);
	char* data = receive_from_socket(client_fd);
	printf("%s", data);

	close(server_fd);
	close(client_fd);

	free(data);
	free(server_address);


	return 0;
}