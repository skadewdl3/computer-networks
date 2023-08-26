#include "socket.h"

char* add_flag_byte (char* data) {
	char flag[] = "01111110";
	size_t data_length = strlen(data);
	size_t flag_length = strlen(flag);
	size_t copy_length = data_length + 2 * flag_length;
	char* copy = (char*)malloc(sizeof(char) * (copy_length + 1));

	strcat(copy, flag);
	strcat(copy, data);
	strcat(copy, flag);
	copy[copy_length] = '\0';

	return copy;
} 


void strslice (char* dest, char* src, size_t begin, size_t count) {
	size_t src_length = strlen(src);
	for (size_t i = begin; i < (count == -1 ? src_length : begin + count); i++) {
		dest[i - begin] = src[i];
	}
}

char* bit_stuffing (char* data) {
	size_t length = strlen(data);
	char* copy = (char*)malloc(sizeof(char) * length);
	char* temp = (char*)malloc(sizeof(char) * length);

	strcpy(copy, data);

	for (size_t i = 4; i < length; i++) {
		if (data[i] == '1' && data[i - 1] == '1' && data[i - 2] == '1' && data[i - 3] == '1' && data[i - 4] == '1') {
			strslice(temp, copy, i + 1, -1);
			copy = realloc(copy, sizeof(char) * (strlen(copy) + 1));
			copy[i + 1] = '0';
			copy[i + 2] = '\0';
			strcat(copy, temp);
		}
	}

	free(temp);
	return copy;
}

char* byte_stuffing (char* data) {
	size_t length = strlen(data);
	char* copy = (char*)malloc(sizeof(char) * length);
	char* temp = (char*)malloc(sizeof(char) * length);
	char* temp2 = (char*)malloc(sizeof(char) * 9);

	size_t copy_length = length;

	temp2[8] = '\0';
	char esc[] = "00011011";
	char flag[] = "01111110";

	strcpy(copy, data);
	int skip = 0;

	for (size_t i = 7; i < length; i++) {
		strslice(temp2, data, i - 7, 8);
		if (strcmp(temp2, esc) == 0 || strcmp(temp2, flag) == 0) {
			printf("\n\n%ld", i);
			strslice(temp, copy, i - 7, -1);
			copy[i - 7] = '\0';
			strcat(copy, esc);
			strcat(copy, temp);
			copy_length += 8;
			
		}
	}

	return copy;
}

int main () {

	char data[] = "_01111110_01111110_";
	char* flagged = add_flag_byte(data);
	// char* bit_stuffed = bit_stuffing(data);
	char* byte_stuffed = byte_stuffing(data);


	printf("\n%s", byte_stuffed);

/*
	int client_fd = create_socket(AF_INET, SOCK_STREAM, AUTO);
	Address* server_address = create_address(AF_INET, 8080);
	set_socket_options(client_fd);
	connect_to_address(client_fd, server_address);
	fflush(stdout);

	send_on_socket(client_fd, "\nHello from the client.");

	close(client_fd);

	free(server_address);
*/

	free(flagged);
	// free(bit_stuffed);
	return 0;
}