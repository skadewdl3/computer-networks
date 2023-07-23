#include <stdio.h>	
#include "sockets.h"

int main () {

	Socket* client_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	
	add_address(client_socket, 8080);
	create_connection(client_socket);

	Response* message = receive_on_socket(client_socket);
	printf("%f", parse_float(message));

	return 0;
}