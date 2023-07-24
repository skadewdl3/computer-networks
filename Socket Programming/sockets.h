#include "sockets-common.h"
#include "sockets-response.h"
#include "sockets-default.h"
#include "sockets-master.h"

#define close_socket(socket) _Generic((socket),                               \
     Socket*: close_socket_default,                                               \
     MasterSocket*: close_socket_master                                                    \
)(socket)

#define add_address(socket, port) _Generic((socket),                               \
     Socket*: add_address_default,                                               \
     MasterSocket*: add_address_master                                                    \
)(socket, port)

#define bind_socket(socket) _Generic((socket),                               \
     Socket*: bind_socket_default,                                               \
     MasterSocket*: bind_socket_master                                                    \
)(socket)

#define listen_on_socket(socket, callback) _Generic((socket),                               \
     Socket*: listen_on_socket_default,                                               \
     MasterSocket*: listen_on_socket_master                                                    \
)(socket, callback)

#define accept_connection_on_socket(socket) _Generic((socket),                               \
     Socket*: accept_connection_on_socket_default,                                               \
     MasterSocket*: accept_connection_on_socket_master                                                    \
)(socket)

#define send_to_socket(socket, data) _Generic((data),                               \
     int: send_to_socket_int,                                                             \
     float: send_to_socket_float,                                                             \
     double: send_to_socket_float,                                                             \
     char: send_to_socket_char,                                                             \
     char*: send_to_socket                                                             \
)(socket, data)

#define receive_on_socket(socket) _Generic((socket),                               \
     Socket*: receive_on_socket_default,                                               \
     MasterSocket*: receive_on_socket_master                                                    \
)(socket)

#define destroy(item) _Generic((item),                               \
     Socket*: close_socket_default,                                               \
     MasterSocket*: close_socket_master,                                                    \
     Response*: destroy_response,                                                    \
     Connections*: destroy_connection_list,                                                    \
     default: free(item)                                                    \
)(item)
