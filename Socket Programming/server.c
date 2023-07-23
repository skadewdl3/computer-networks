// Standard libraries
#include <stdio.h>
#include "sockets.h"

int main () {
	Socket* server_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	add_address(server_socket, 8080);
	bind_socket(server_socket);
	listen_on_socket(server_socket);
	Socket* client_socket = accept_connection_on_socket(server_socket);
	send_to_socket(client_socket, "Hello from the server!" );

	close_socket(server_socket);
	close_socket(client_socket);
}