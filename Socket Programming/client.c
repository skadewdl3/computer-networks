#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
 #include <unistd.h>

// A function to throw an error and exit the program
void error (char* message) {
	perror(message);
	exit(1);
}

// A function to throw an error and exit the program
void success (char* message) {
	printf("%s", message);
}

int main () {

	int port_no = 8081;

	struct sockaddr_in server_address;

	int client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket < 0) {
		error("\nError in creating socket.");
	}
	else {
		success("\nClient socket created successfully.");
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port_no);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int connection_status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

	if (connection_status == -1) {
		error("\nConnection failed.");
	}
	else {
		success("\nConnection successful.");
	}

	char server_response[1000];
	recv(client_socket, &server_response, sizeof(server_response), 0);

	printf("Message from server: %s", server_response);

	close(client_socket);

	return 0;
}