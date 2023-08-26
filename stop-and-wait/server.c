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

typedef struct {
	char key[1000];
	char checksum[1000];
} Packet;

int main () {

	int server_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(server_fd);
	bind_socket(server_fd, server_address);
	listen_on_socket(server_fd, 5);
	int client_fd = accept_connection(server_fd, server_address);

	while (1) {

		Packet to_recv;
		Packet ack;

		receive_in_buffer(client_fd, &to_recv, sizeof(Packet));
		printf("\nKey: %s\n\nChecksum: %s\n\n", to_recv.key, to_recv.checksum);


		size_t length = strlen(to_recv.checksum);

		char zeros[length + 1];
		zeros[0] = '\0';
		for (size_t i = 0; i < length; i++) {
			strcat(zeros, "0");
		}
		zeros[length] = '\0';

		binary_division(to_recv.checksum, to_recv.key);

		printf("\n\n%s\n\n%s", to_recv.checksum, zeros);

		if (strcmp(to_recv.checksum, zeros) == 0) {
			strcpy(ack.checksum, "Checksum valid. No errors.");
		}
		else {
			strcpy(ack.checksum, "Checksum is invalid. Error in data.");
		}
		send_buffer(client_fd, &ack, sizeof(Packet));
		fflush(stdout);


	}
	close(client_fd);
	close(server_fd);
	free(server_address);


	return 0;
}