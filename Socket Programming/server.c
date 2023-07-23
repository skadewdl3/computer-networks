// Standard libraries
#include <stdio.h>
#include "sockets.h"

int main () {
	Socket* server_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	MasterSocket* master = create_master_socket(INTERNET, STREAM_SOCKET, AUTO, 10);

	add_address(server_socket, 8080);
	bind_socket(server_socket);
	listen_on_socket(server_socket, 5);

	Socket* client_socket = accept_connection_on_socket(server_socket);
	send_to_socket(client_socket, "10.987" );

	close_socket(server_socket);
	close_socket(client_socket);
	close_socket(master);
}