#include "helpers.h"

const int ROOT_ID = -1;
const int SRC_PORT = 5555;
const char SOCKET_PATTERN[] = "tcp://127.0.0.1:";

char *solve_address(int id) {
  char *buf = (char *)malloc(sizeof(SOCKET_PATTERN) + 10);
  sprintf(buf, "%s%d", SOCKET_PATTERN, id + SRC_PORT);
  return buf;
}

void *init_socket(void *context, int id, int type) {
  void *socket = zmq_socket(context, type);
  char *address = solve_address(id);
  switch (type) {
    case ZMQ_PULL:
      if (zmq_bind(socket, address)) perror("zmq_bind");
      break;
    case ZMQ_PUSH:
      if (zmq_connect(socket, address)) perror("zmq_connect");
      break;
  }
  free(address);
  return socket;
}

int send_message(void *pusher, char *buffer, size_t size) {
  if (zmq_send(pusher, buffer, size, ZMQ_DONTWAIT) == -1) {
    perror("zmq_send");
    return 0;
  }
  return 1;
}
