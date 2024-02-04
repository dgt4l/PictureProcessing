#ifndef UTIL_ZMQ_HELPERS
#define UTIL_ZMQ_HELPERS

#include <stdlib.h>
#include <string.h>
#include <zmq.h>
#include <sys/types.h>


extern const int ROOT_ID;
extern const int SRC_PORT;
extern const char SOCKET_PATTERN[];

char *solve_address(int id);
void *init_socket(void *context, int id, int type);
int send_message(void *pusher, char *buffer, size_t size);

#endif  // UTIL_ZMQ_HELPERS