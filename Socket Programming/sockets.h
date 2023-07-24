#include "sockets-logic.h"
#define close_socket(socket) _Generic((socket),                               \
     Socket*: close_default_socket,                                               \
     MasterSocket*: close_master_socket                                                    \
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

#define receive_on_socket(socket) _Generic((socket),                               \
     Socket*: receive_on_socket_default,                                               \
     MasterSocket*: receive_on_socket_master                                                    \
)(socket)
