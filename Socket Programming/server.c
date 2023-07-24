// Standard libraries
#include <stdio.h>
#include "sockets.h"

void listener (Socket* sock, Response* response, StopSocketListening stop, CloseConnection close) {
	printf("received: %s\n", parse_str(response));
	send_to_socket(sock, "Hello from the server!");
}

int main () {
	MasterSocket* master = create_master_socket(INTERNET, STREAM_SOCKET, AUTO);
	Connections* conn = create_connection_list(10);
	add_address(master, 8080);
	add_connection_list(master, conn);
	bind_socket(master);

	listen_on_socket(master, listener);

	close_socket(master);
}