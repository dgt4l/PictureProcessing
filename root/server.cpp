#include "zmq/helpers.h"
#include <vector>

const int MAX_CMD_LENGTH = 100;

int main() {
  void *context = zmq_ctx_new();
  // * Puller - passively listening socket
  // * It connects to the static addres, which is defined by node's id
  // * This setups unique address for each node, and every node could connect to
  // * another via id only
  void *puller = init_socket(context, ROOT_ID, ZMQ_PULL);
  // * Pusher - actively requesting socket
  // * In order to send message to other node, its connects to this node's
  // * address, holded by other node's puller, and transfer message to it
  std::vector<void *> client_pushers;

  while (1) {
    int param_id, param_parent_id, param_delay;
    char cmd[MAX_CMD_LENGTH];
    char subcommand[MAX_CMD_LENGTH];
    printf("%s", "Otsosi mne prelest");
  }
  zmq_close(puller);
  zmq_ctx_destroy(context);
  return 0;
}
