#include "handlers/command_dispatcher.h"
#include "handlers/response_handler.h"
#include "server.h"
#include "../utils/socket_pattern.h"

const char SERVER_PREFIX[] = "[Server]";


WorkerMap worker_map;
zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::pull);

static int GLOBAL_ID = 1;

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

void common_picture_processing(std::string path, int width, int height) {
  std::string id = std::to_string(GLOBAL_ID++);
  CommandDispatcher::getInstance().dispatch_command("CREATE " + id);
  int amount = ResourseManager::calculate_resources(width, height);
  if (ResourceManager::delegate_resources()) {

  }
}