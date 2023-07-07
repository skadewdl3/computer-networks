#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char xor (char a, char b) {
  if (a == b) return '0';
  else return '1';
}

char* binary_division (char* appended, char* key, int data_length, int key_length) {
  char* copy = (char*)malloc((data_length + key_length - 1) * sizeof(char));
  for (int i = 0; i < data_length + key_length - 1; i++) copy[i] = appended[i];
  for (int i = 0; i < data_length; i++) {
    if (copy[i] == '1') {
      for (int j = 0; j < key_length; j++) {
        copy[i+j] = xor(copy[i+j], key[j]);
      }
    }
  }
  char* checksum = (char*)malloc(key_length * sizeof(char));
  for (int i = 0; i < key_length - 1; i++) checksum[i] = copy[data_length + i];
  checksum[key_length - 1] = '\0';
  return checksum;
}

char* sender (char* data, char* key) {
  int data_length = strlen(data);
  int key_length = strlen(key);

  if (key_length > data_length) {
    printf("Error! Key length is greater than data length.\n");
    exit(1);
    return NULL;
  }

  int data_appended_length = data_length + key_length - 1;
  char *data_appended = (char*)malloc(data_appended_length * sizeof(char));
  for (int i = 0; i < data_length; i++) data_appended[i] = data[i];
  for (int i = data_length; i < data_appended_length; i++) data_appended[i] = '0';

  char* checksum = binary_division(data_appended, key, data_length, key_length);
  for (int i = 0; i < key_length - 1; i++) data_appended[data_length + i] = checksum[i];
  data_appended[data_appended_length] = '\0';
  printf("Message: %s\n", data);
  printf("CRC Key: %s\n", key);
  printf("Checksum: %s\n\n", checksum);
  return data_appended;
}

void maybe_cause_error (char* data_appended, int data_appended_length) {
  srand(time(NULL));
  // int should_cause_error = 1;
  int should_cause_error = rand() % 2;
  if (!should_cause_error) return;
  srand(time(NULL));
  int random = rand() % data_appended_length;
  if (data_appended[random] == '0') data_appended[random] = '1';
  else data_appended[random] = '0';
  printf("Error! Checksum changed to: %s\n\n", data_appended);
}

int receiver (char* data_appended, char* key) {
  int data_appended_length = strlen(data_appended);
  int key_length = strlen(key);
  printf("Checksum received: %s\n", data_appended);
  char* result = binary_division(data_appended, key, data_appended_length - key_length + 1, key_length);

  int ones = 0;
  for (int i = 0; i < strlen(result); i++) {
    if (result[i] == '1') ones++;
  }
  if (ones == 0) {
    printf("Checksum verified. Data is valid.\n");
    printf("Original message is: ");
    for (int i = 0; i < data_appended_length - key_length + 1; i++) {
      printf("%c",data_appended[i]);
    } 
    return 1;
  }
  else {
    printf("Checksum invalid. Error has occurred.\n");
    return 0;
  }
}

char* get_string_input(char* message) {
  char* input = (char*)malloc(sizeof(char) * 1000);
  printf("%s", message);
  scanf("%[^\n]%*c", input);
  input = realloc(input, (strlen(input) + 1) * sizeof(char));
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] != '0' && input[i] != '1') {
      return get_string_input("Please enter a valid binary bitstring: ");
    }
  }
  return input;
}

int main () {

  char* data = get_string_input("Enter message to send: ");
  char* key = get_string_input("Enter CRC key: ");
  

  char* data_appended = sender(data, key);
  maybe_cause_error(data_appended, strlen(data_appended));
  receiver(data_appended, key);

  return 0;
}