#include <stdio.h>	
#include "sockets.h"

int main () {

	Socket* client_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	
	add_address(client_socket, 8080);
	create_connection(client_socket);

	char* message = receive_on_socket(client_socket);
	printf("%s", message);

	return 0;
}