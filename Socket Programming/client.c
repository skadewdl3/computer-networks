#include <stdio.h>	
#include "sockets.h"

int is_valid_operation (char operation) {
	return operation == '+' || operation == '-' || operation == '*' || operation == '/' || operation == '%';
}

int main () {

	Socket* client_socket = create_socket(INTERNET, STREAM_SOCKET, AUTO);
	add_address(client_socket, 8080);
	create_connection(client_socket);


	char operation;
	float operand1;
	float operand2;
	while (1) {
trap:
		printf("\nEnter operation (+, -, *, /, %s): ", "%");
		scanf("%c%*c", &operation);
		if (!is_valid_operation(operation)) goto trap;
		send_to_socket(client_socket, operation);
		printf("\nEnter a: ");
		scanf("%f%*c", &operand1);
		send_to_socket(client_socket, operand1);
		printf("\nEnter b: ");
		scanf("%f%*c", &operand2);
		send_to_socket(client_socket, operand2);
		Response* response = receive_on_socket(client_socket);
		printf("Result: %f", parse_float(response));
		destroy(response);
	}

	close_socket(client_socket);
	return 0;
}