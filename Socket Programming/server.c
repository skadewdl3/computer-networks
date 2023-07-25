#include <stdio.h>
#include "sockets.h"

int is_valid_operation (char operation) {
	return operation == '+' || operation == '-' || operation == '*' || operation == '/' || operation == '%';
}

float calculate (char operation, float a, float b) {
	if (operation == '+') {
		return a + b;
	}
	else if (operation == '-') {
		return a - b;
	} 
	else if (operation == '*') {
		return a * b;
	}
	else if (operation == '/') {
		return a / b;
	}
	else if (operation == '%') {
		return (int)a % (int)b;
	}

}

int main () {

	char operation;
	int operand1;
	int operand2;
	int operand_count = 0;


	void listener (Socket* sock, Response* response, StopListening stop, CloseConnection close) {
		if (is_valid_operation(parse_char(response))) {
			operation = parse_char(response);
		}
		else if (operand_count == 0) {
			operand1 = parse_float(response);
			operand_count++;
		}
		else if (operand_count == 1) {
			operand2 = parse_float(response);
			operand_count = 0;
			float result = calculate(operation, operand1, operand2);
			send_to_socket(sock, result);
		}
	}


	MasterSocket* master = create_master_socket(INTERNET, STREAM_SOCKET, AUTO);
	Connections* conn = create_connection_list(10);
	add_address(master, 8080);
	add_connection_list(master, conn);
	bind_socket(master);
	listen_on_socket(master, listener);
	close_socket(master);
}