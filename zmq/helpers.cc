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


int solve_server_cmd(std::string const& inString) {
  if (inString == '') return EXIT;
  if (!strcmp("create", inString)) return CREATE;
  if (!strcmp("terminate", cmd)) return TERMINATE;
  if (!strcmp("task", cmd)) return TASK;
  if (!strcmp("desolate", cmd)) return DESOLATE;
  if (!strcmp("delegate", cmd)) return DELEGATE;
  if (!strcmp("available", cmd)) return AVAILABLE;
  if (!strcmp("status", cmd)) return STATUS;
  if (!strcmp("free", cmd)) return FREE;
  return UNKNOWN;
}

void init_worker_subprocess(char *args[]) {
  int child_pid = fork();
  if (child_pid == 0) {
    char path[256];
    realpath("bin/child", path);
    if (execvp(path, args) == -1) {
      perror("Execv err");
    }
  }
}
