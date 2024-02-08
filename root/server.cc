#include "handlers/command_dispatcher.h"
#include "handlers/response_handler.h"
#include "server.h"
#include "../utils/socket_pattern.h"

// const int MAX_CMD_LENGTH = 100;
// const int BUFFER_SIZE = 2048;
const char SERVER_PREFIX[] = "[Server]";

std::map<int, int> worker_map;

zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::pull);

void command_dispatcher_thread() {
  while (CommandDispatcher::getInstance().dispatch_command())
    ;
}

void response_handler_thread() {
  while (ResponseHandler::getInstance().read_response())
    ;
}

int main() {
  pusher.bind(SERVER_PUSHER_SOCKET_PATTERN);
  puller.bind(SERVER_PULLER_SOCKET_PATTERN);
  
  std::thread cd_thread(command_dispatcher_thread);
  std::thread rh_thread(response_handler_thread);

  cd_thread.join();
  rh_thread.join();

  return 0;
}
