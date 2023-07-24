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