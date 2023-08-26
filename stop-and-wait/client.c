#include "socket.h"
#include <time.h>
#include <math.h>

char xor (char a, char b) {
	return a == b ? '0' : '1';
}

char not (char a) {
	return a == '0' ? '1' : '0';
}

float random_num (long limit) {
	srand(time(NULL));
	return rand() % limit;
}

void maybe_cause_error (char* checksum) {
	float should_cause_error = random_num(10);
	if (should_cause_error <= 5) {
		printf("\nNot causing error");
		return;
	}
	printf("\ncausing error");
	size_t length = strlen(checksum);
	long c = floor(random_num(length));
	checksum[c] = not(checksum[c]);
	printf("\n%ld\n", c);
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

char* CRC (char* string, char* key) {
	size_t length = strlen(string);
	size_t key_length = strlen(key);
	size_t checksum_length = length + key_length - 1;

	char* checksum = (char*)malloc(sizeof(char) * (checksum_length + 1));
	char* temp = (char*)malloc(sizeof(char) * key_length);

	strcat(checksum, string);
	for (size_t i = 0; i < key_length - 1; i++) strcat(checksum, "0");
	checksum[checksum_length] = '\0';


	binary_division(checksum, key);
	strslice(temp, checksum, length, key_length - 1);
	checksum[0] = '\0';
	strcat(checksum, string);
	strcat(checksum, temp);
	return checksum;

}

typedef struct {
	char key[1000];
	char checksum[1000];
} Packet;

int main () {

	int client_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(client_fd);
	connect_to_address(client_fd, server_address);
	fflush(stdout);

	while (1) {
		char string[1000];
		char key[1000];
		Packet to_send;
		Packet to_recv;

		printf("Enter binary string: ");
		scanf("%[^\n]%*c", string);

		printf("Enter key: ");
		scanf("%[^\n]%*c", key);

		char* checksum = CRC(string, key);
		maybe_cause_error(checksum);
		strcpy(to_send.key, key);
		strcpy(to_send.checksum, checksum);
		send_buffer(client_fd, &to_send, sizeof(Packet));

		receive_in_buffer(client_fd, &to_recv, sizeof(Packet));
		printf("Result is: ");
		printf("%s\n\n", to_recv.checksum);
		fflush(stdout);
	}


	close(client_fd);
	free(server_address);
	return 0;
}