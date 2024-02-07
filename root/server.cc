#include "../zmq/command_dispatcher.h"

const int MAX_CMD_LENGTH = 100;
const int BUFFER_SIZE = 2048;
const char SERVER_PREFIX[] = "[Server]";

zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::push);

int main() {
  pusher.bind(SERVER_PUSHER_SOCKET_PATTERN);
  while (CommandDispatcher::getInstance().dispatch_command())
    ;

  return 0;
}
