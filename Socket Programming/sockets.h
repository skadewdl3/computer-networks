#include "sockets-logic.h"
#define close_socket(socket) _Generic((socket),                               \
     Socket*: close_default_socket,                                               \
     MasterSocket*: close_master_socket                                                    \
)(socket)
