#define SUPPRESS_SOCKET_LOGS
#include "socket.h"

char xor (char a, char b) {
	return a == b ? '0' : '1';
}

void strslice (char* dest, char* src, size_t begin, size_t count) {
	size_t length = strlen(src);
	for (size_t i = begin; i < (count == -1 ? length : begin + count); i++) {
		dest[i - begin] = src[i]; 
	}
}


void binary_division (char* dividend, char* divisor) {
	size_t dividend_length = strlen(dividend);
	size_t divisor_length = strlen(divisor);

	for (size_t i = 0; i < dividend_length; i++) {
		if (dividend[i] == '1') {
			for (size_t j = 0; j < divisor_length; j++) {
				dividend[i + j] = xor(dividend[i + j], divisor[j]);
			}
		}
	}
}

int main () {

	int server_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(server_fd);
	bind_socket(server_fd, server_address);
	listen_on_socket(server_fd, 5);
	int client_fd = accept_connection(server_fd, server_address);


	while (1) {

		char* key = receive_from_socket(client_fd);
		send_on_socket(client_fd, "Received key");
		char* checksum = receive_from_socket(client_fd);

		printf("\nKey: %s\n\nChecksum: %s\n\n", key, checksum);

		size_t length = strlen(checksum);

		char zeros[length + 1];
		zeros[0] = '\0';
		for (size_t i = 0; i < length; i++) {
			strcat(zeros, "0");
		}
		zeros[length] = '\0';

		binary_division(checksum, key);

		printf("\n\n%s\n\n%s", checksum, zeros);

		if (strcmp(checksum, zeros) == 0) {
			send_on_socket(client_fd, "Checksum is valid. No error in data.");
		}
		else {
			send_on_socket(client_fd, "Checksum is invalid. Error in data.");
		}

		fflush(stdout);
	}

	close(server_fd);
	close(client_fd);
	free(server_address);


	return 0;
}