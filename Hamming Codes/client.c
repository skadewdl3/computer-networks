#include "../sockets-lib/sockets.h"
#include <stdio.h>
#include <math.h>
int is_hamming_position (size_t pos) {
    return pos == 0 || pos == 1 || pos == 3 || pos == 7;
}

char* create_hamming_string (char* message) {
    if (message == NULL) {
        return NULL;
    }
    size_t length = strlen(message);
    if (length > 11) {
        printf("message is too long");
        return NULL;
    }
    if (length < 11) {
        printf("message is too short");
        return NULL;
    }
    char* hamming_code = malloc(sizeof(char) * (length + 4));
    size_t hamming_length = length + 4;
    hamming_code[hamming_length] = '\0';
    for (int i = 0, j = 0; i < hamming_length; i++) {
        if (is_hamming_position(i)) {
            hamming_code[i] = '_';
        } else {
            hamming_code[i] = message[j];
            j++;
        }
    }

    int skip_width = 1;
    int skipped = 0;

    for (int i = 0; i < 4; i++) {
        int ones = 0;
        for (int j = 0; j < length; j++) {
            if (!skipped) {
                j += skip_width;
                skipped = 1;
            }
            else {
                for (int k = 0; k < skip_width; k++) {
                    if (message[j + k] == '1') ones++;
                }
            }
        }
        if (ones % 2 == 1) {
            hamming_code[(int)pow(2, i)] = '1';
        }
        else {
            hamming_code[(int)pow(2, i)] = '0';
        }
    }

//    int hamming_pos_1[6] = {1, 2, 4, 6, 8, 10};
//    int hamming_pos_2[6] = {1, 2, 5, 6, 9, 10};
//    int hamming_pos_3[4] = {3, 4, 5, 6};
//    int hamming_pos_4[4] = {7, 8, 9, 10};
//
//    int ones = 0;
//
//    for (int i = 0; i < 6; i++) {
//        if (message[hamming_pos_1[i]] == '1') ones++;
//    }
//    hamming_code[0] = ones % 2 == 0 ? '0' : '1';
//    ones = 0;
//
//    for (int i = 0; i < 6; i++) {
//        if (message[hamming_pos_2[i]] == '1') ones++;
//    }
//    hamming_code[1] = ones % 2 == 0 ? '0' : '1';
//    ones = 0;
//
//    for (int i = 0; i < 4; i++) {
//        if (message[hamming_pos_3[i]] == '1') ones++;
//    }
//    hamming_code[3] = ones % 2 == 0 ? '0' : '1';
//    ones = 0;
//
//    for (int i = 0; i < 4; i++) {
//        if (message[hamming_pos_4[i]] == '1') ones++;
//    }
//    hamming_code[7] = ones % 2 == 0 ? '0' : '1';

    return hamming_code;
}

int main()
{

//    Socket *client = create_socket(INTERNET, STREAM_SOCKET, AUTO);
//    add_address(client, 8080);
//    create_connection(client);
    char message[] = "00000111111";
    printf("Original: %s\n", message);

    char* hamming_code = create_hamming_string(message);
    printf("Hamming Code: %s\n", hamming_code);
//    send_to_socket(client, hamming_code);
    return 0;
}