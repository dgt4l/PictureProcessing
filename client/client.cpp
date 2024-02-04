#include "zmq/helpers.h"
#include <unistd.h>

void imalive(int id) { printf("%s %d\n", "[Success]", getpid()); }

int main(int argc, char *argv[]) {
  int id = atoi(argv[1]);
  void *context = zmq_ctx_new();
  void *puller = init_socket(context, id, ZMQ_PULL);
  void *root_pusher = init_socket(context, ROOT_ID, ZMQ_PUSH);
  void *sibling_pusher = NULL;
  void *son_pusher = NULL;

  imalive(id);
  char buffer[1000];
  char subcommand[1000], cmd[1000];
  int size = 0, param_id, param_parent_id, param_sibling_id, param_son_id,
      param_delay;

  while (1) {
    size = zmq_recv(puller, buffer, sizeof(buffer) + 2, ZMQ_DONTWAIT);
    if (size != -1) {
      printf("[%d] Recieved command: %s\n", id, buffer);
    }
  }
  return 0;
}

