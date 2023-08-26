#define SUPPRESS_SOCKET_LOGS
#include "socket.h"


double pow (double a, double b) {
	double result = 1;
	for (unsigned long int i = 0; i < b; i++) {
		if (b > 0) {
			result *= a;
		}
		else if (b < 0) {
			result /= a;
		}
	}
	return result;
}

double calculate (double n1, char operator, double n2) {
	switch (operator) {
		case '+':
			return n1 + n2;
		case '-':
			return n1 - n2;
		case '*':
			return n1 * n2;
		case '/':
			return n1 / n2;
		case '^':
			return pow(n1, n2);
		default:
			return -1;
	}
}

int main () {

	int server_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(server_fd);
	bind_socket(server_fd, server_address);
	listen_on_socket(server_fd, 5);
	int client_fd = accept_connection(server_fd, server_address);


	double n1;
	double n2;
	char operator;
	short arg_count = 0;


	while (1) {

		char* n1_data = receive_from_socket(client_fd);
		n1 = atof(n1_data);
		free(n1_data);

		char* n2_data = receive_from_socket(client_fd);
		n2 = atof(n2_data);
		free(n2_data);

		char* operator_data = receive_from_socket(client_fd);
		operator = operator_data[0];
		free(operator_data);


		char result[1000];
		gcvt(calculate(n1, operator, n2), 10, result);

		printf("\nResult with (%c) is: %s", operator, result);
		fflush(stdout);

		send_on_socket(client_fd, result);
	}

	close(server_fd);
	close(client_fd);
	free(server_address);


	return 0;
}