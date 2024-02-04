#ifndef UTIL_ZMQ_HELPERS
#define UTIL_ZMQ_HELPERS

#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>


extern const int ROOT_ID;
extern const int SRC_PORT;
extern const char SOCKET_PATTERN[];

char *solve_address(int id);
void *init_socket(void *context, int id, int type);
int send_message(void *pusher, char *buffer, size_t size);

int solve_server_cmd(std::string const& buffer);
void init_worker_subprocess(char *args[]);

enum SERVER_STATUS {
    CLOSED,
    ONLINE
};

enum CMD_CODES {
  EXIT = -1,
  UNKNOWN,
  CREATE,
  TERMINATE,
  TASK,
  DESOLATE,
  DELEGATE,
  AVAILABLE,
  STATUS,
  FREE
};

enum WORKER_STATUS {
    IDLE,
    READY,
    WORKING,
    PAUSED,
    COMPLETE
};

#endif  // UTIL_ZMQ_HELPERS