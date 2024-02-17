#include "handlers/command_dispatcher.h"
#include "handlers/response_handler.h"
#include "server.h"
#include "../utils/socket_pattern.h"

const char SERVER_PREFIX[] = "[Server]";


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

void common_picture_processing(ResourseManager &rm_, std::string path, std::string filter, int width, int height) {
  int id = GLOBAL_ID++;
  CommandDispatcher::getInstance().dispatch_command("CREATE " + std::to_string(id));
  while (!worker_map.is_reserved(id))
    ;
  CommandDispatcher::getInstance().dispatch_command("EXEC TASK " + std::to_string(id) + " " + path + " " + filter);
  int limit = rm_.calculate_limit(width, height); // ? return 5
  int hard_limit = rm_.calculate_hard_limit(width, height);
  ResourseManager::STRATEGY_TYPE strategy = rm_.delegate_resources(limit, hard_limit, id);
  switch (strategy) {
    case ResourseManager::STRATEGY_TYPE::NONE: {
      CommandDispatcher::getInstance().dispatch_command("EXEC DELEGATE " + std::to_string(id) + " " + std::to_string(limit));
      break;
    }
    
    case ResourseManager::STRATEGY_TYPE::DESOLATE: {
      // * simply casts desolate directive to all workers
      ResponseHandler::getInstance().set_resource_collecting(limit, strategy);
      CommandDispatcher::getInstance().dispatch_command("EXEC DESOLATE");
      // * wait till resource collecting ends
      while (ResponseHandler::getInstance().is_collecting_now)
        ;
      ResourseManager::STRATEGY_TYPE strategy = rm_.delegate_resources(limit, hard_limit, id);
      CommandDispatcher::getInstance().dispatch_command("EXEC DELEGATE " + std::to_string(id) + " " + std::to_string(limit));
      break;
    }

    case ResourseManager::STRATEGY_TYPE::SURRENDER: {
      // * return by inner channels about failed attempt to allocate resources
      std::cout << "Gave up allocating this much resources: " << std::to_string(limit) << std::endl;
      break;
    }

    case ResourseManager::STRATEGY_TYPE::QUENED: {
      // * notify user about quening
      break;
    }

    case ResourseManager::STRATEGY_TYPE::DIVIDE: {
      // * do divide tricks
      break;
    }
  }
}

int main() {
  rm.set_capacity(10);

  pusher.bind(SERVER_PUSHER_SOCKET_PATTERN);
  puller.bind(SERVER_PULLER_SOCKET_PATTERN);
  
  std::thread cd_thread(command_dispatcher_thread);
  std::thread rh_thread(response_handler_thread);
  
  common_picture_processing(rm, "../materials/arbuz.png", "NEGATIVE", 1, 1);
  common_picture_processing(rm, "../materials/50.png", "NEGATIVE", 1, 1);
  sleep(3);
  common_picture_processing(rm, "../materials/arbuz.png", "NEGATIVE", 1, 1);
  common_picture_processing(rm, "../materials/50.png", "NEGATIVE", 1, 1);

  cd_thread.join();
  rh_thread.join();

  return 0;
}

