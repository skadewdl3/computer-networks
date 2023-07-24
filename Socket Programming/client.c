#include <stdio.h>	
#include "sockets.h"

int main () {

	Socket* client_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	
	add_address(client_socket, 8080);
	create_connection(client_socket);

	send_to_socket(client_socket, "Ok bro");
	char* res = parse_str(receive_on_socket(client_socket));
	printf("\n%s", res);
	// send_to_socket(client_socket, "Ok Bro");

	close_socket(client_socket);

	return 0;
}