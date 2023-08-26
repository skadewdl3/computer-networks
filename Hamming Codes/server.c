#include "../sockets-lib/sockets.h"
#include <stdio.h>
#include <math.h>

int main()
{
    Socket *server = create_socket(INTERNET, STREAM_SOCKET, AUTO);
    add_address(server, 8080);
    bind_socket(server);
    listen_on_socket(server);
    Socket* client = accept_connection_on_socket(server);
    Response* res = receive_on_socket(client);
    int redundant_bit_positions[] = {0, 1, 3, 7};
    int sum = 0;

    char* hamming_code = parse_str(res);

    printf("code: %s", hamming_code);
    return 0;
}