#include <stdlib.h>

typedef enum {
	StringRepsonse, IntegerResponse, DecimalResponse 
} ResponseType;

typedef struct {
	char* data;
	int status;
} Response;

char* parse_str (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as string - response is invalid.");
		return NULL;
	}
	return response->data;
}
int parse_int (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as int - response is invalid.");
		return -1;
	}
	return atoi(response->data);
}

double parse_float (Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as float - response is invalid.");
		return -1;
	}
	return atof(response->data);
}

char parse_char(Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as float - response is invalid.");
		return -1;
	}
	return response->data[0];
}


long parse_long(Response* response) {
	if (response->data == NULL) {
		error("\nError while reading response as float - response is invalid.");
		return -1;
	}
	return atol(response->data);
}

char* int_to_str (int data) {
	char* str = (char*)malloc(DEFAULT_BUFFER_LENGTH * sizeof(char));
	sprintf(str, "%d", data);
	int length = strlen(str);
	str = realloc(str, (length + 1) * sizeof(char));
	str[length] = '\0';
	return str;
}

char* float_to_str (float data) {
	char* str = (char*)malloc(DEFAULT_BUFFER_LENGTH * sizeof(char));
	gcvt(data, 10, str);
	int length = strlen(str);
	str = realloc(str, (length + 1) * sizeof(char));
	str[length] = '\0';
	return str;
}

char* char_to_str (char data) {
	char* str = (char*)malloc(2 * sizeof(char));
	str[0] = data;
	str[1] = '\0';
	return str;
}

char* str_to_str (char* str) {
	return str;
}

char* long_to_str (long data) {
	char* str = (char*)malloc(DEFAULT_BUFFER_LENGTH * sizeof(char));
	sprintf(str, "%ld", data);
	int length = strlen(str);
	str = realloc(str, (length + 1) * sizeof(char));
	str[length] = '\0';
	return str;
} 