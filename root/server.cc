#include "handlers/command_dispatcher.h"
#include "handlers/response_handler.h"
#include "server.h"
#include "../utils/socket_pattern.h"

const char SERVER_PREFIX[] = "\e[32m[Server]\e[0m\t\t\t ";

WaitingQueue wq;
WorkerMap worker_map;
ResourseManager rm;
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

void common_picture_processing(std::string path, std::string filter, int width, int height) {
  int id = GLOBAL_ID++;
  CommandDispatcher::getInstance().dispatch_command("CREATE " + std::to_string(id));
  while (!worker_map.is_reserved(id))
    ;
  CommandDispatcher::getInstance().dispatch_command("EXEC TASK " + std::to_string(id) + " " + path + " " + filter);
  while (!worker_map.is_gotwork(id))
    ;
  int hard_limit = rm.calculate_hard_limit(width, height);
  int limit = std::max(rm.calculate_balance_space(), hard_limit);
  ResourseManager::STRATEGY_TYPE strategy = rm.delegate_resources(limit, hard_limit, id);
  ResponseHandler::getInstance().solve_worker_fate(strategy, id);
}

int main() {
  rm.set_capacity(50);

  pusher.bind(SERVER_PUSHER_SOCKET_PATTERN);
  puller.bind(SERVER_PULLER_SOCKET_PATTERN);
  
  std::thread cd_thread(command_dispatcher_thread);
  std::thread rh_thread(response_handler_thread);
  
  common_picture_processing("../materials/arbuz.png", "NEGATIVE", 1, 1);
  common_picture_processing("../materials/50.png", "NEGATIVE", 1, 1);
  common_picture_processing("../materials/arbuz.png", "NEGATIVE", 1, 1);
  common_picture_processing("../materials/kisa.png", "NEGATIVE", 1, 1);
  common_picture_processing("../materials/blur.png", "NEGATIVE", 1, 1);
  cd_thread.join();
  rh_thread.join();
  return 0;
}
