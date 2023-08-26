#define SUPPRESS_SOCKET_LOGS
#include "socket.h"

int main () {


	int client_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(client_fd);
	connect_to_address(client_fd, server_address);
	fflush(stdout);

	char n1[1000];
	char n2[1000];
	char operator[1];

	while (1) {
		printf("Enter first number: ");
		scanf("%[^\n]%*c", n1);
		send_on_socket(client_fd, n1);

		printf("Enter second number: ");
		scanf("%[^\n]%*c", n2);
		send_on_socket(client_fd, n2);

		printf("Enter operator (+, -, *, /, ^): ");
		scanf("%c%*c", operator);
		send_on_socket(client_fd, operator);

		char* data = receive_from_socket(client_fd);
		printf("Result is: ");
		printf("%s\n\n", data);
		fflush(stdout);
	}


	close(client_fd);
	free(server_address);

	return 0;
}