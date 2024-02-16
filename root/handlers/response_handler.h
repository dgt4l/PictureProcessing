#ifndef UTIL_ZMQ_HELPERS_RESPONSE_HANDLER
#define UTIL_ZMQ_HELPERS_RESPONSE_HANDLER

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <zmq.hpp>

#include "../../lib/cppzmq/zmq_helpers.hpp"
#include "../manager/manager.h"
#include "../utils/auto_handler.h"
#include "../utils/miscellaneous.h"
#include "command_dispatcher.h"

extern zmq::socket_t puller;
extern std::string response;

class ResponseHandler {
 public:
  struct ResourceCollecting {
    int current = 0;
    const int target;
    bool active = false;
    ResourseManager::STRATEGY_TYPE strategy;
    ResourceCollecting() : target(0) {}
    ResourceCollecting(int target_,
                       enum ResourseManager::STRATEGY_TYPE strategy_)
        : target(target_), strategy(strategy_) {}

    bool start_collecting() {
      if (current >= target) return false;
      active = true;
      return true;
    }
  };

 private:
  enum RESPONSE_CODES {
    UNKNOWN,
    IAMALIVE,
    DEAD,
    COMPLETE,
    BUSY,
    TOOSWEET,
    SHORTAGE,
    TRANSFER,
    STATUS
  };
  ResponseHandler() {}
  ResponseHandler(const ResponseHandler&);
  ResponseHandler& operator=(ResponseHandler&);
  const std::list<std::pair<enum RESPONSE_CODES, std::string>> hasher = {
      std::make_pair(UNKNOWN, std::string("UNKNOWN")),
      std::make_pair(IAMALIVE, std::string("IAMALIVE")),
      std::make_pair(DEAD, std::string("DEAD")),
      std::make_pair(COMPLETE, std::string("COMPLETE")),
      std::make_pair(BUSY, std::string("BUSY")),
      std::make_pair(TOOSWEET, std::string("TOOSWEET")),
      std::make_pair(SHORTAGE, std::string("SHORTAGE")),
      std::make_pair(TRANSFER, std::string("TRANSFER")),
      std::make_pair(STATUS, std::string("STATUS"))};
  bool is_collecting_now = false;
  ResourceCollecting rc;

 public:
  static ResponseHandler& getInstance() {
    static ResponseHandler instance;
    return instance;
  }
  int read_response(std::string response);
  std::string ResponseHandler::wait_response(int id, std::string type,
                                             std::string response);
  bool _resource_collecting(int target,
                            enum ResourseManager::STRATEGY_TYPE strategy_) {}

  int desolate_append_amount(ResourceCollecting rc, int amount);
};

#endif  // UTIL_ZMQ_HELPERS_RESPONSE_HANDLER