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
#include <thread>
#include <queue>

#include "../../lib/cppzmq/zmq_helpers.hpp"
#include "manager/manager.h"
#include "../utils/auto_handler.h"
#include "../utils/miscellaneous.h"
#include "command_dispatcher.h"

extern zmq::socket_t puller;
extern ResourseManager rm;
extern WaitingQueue wq;

class ResponseHandler {
 public:
  enum RESPONSE_CODES {
    UNKNOWN,
    IAMALIVE,
    DEAD,
    COMPLETE,
    BUSY,
    TOOSWEET,
    SHORTAGE,
    TRANSFER,
    STATUS,
    GOTWORK
  };
  struct ResourceCollecting {
    int current = 0;
    int target;
    bool active = false;
    int target_id;
    ResourseManager::STRATEGY_TYPE strategy;
    ResourceCollecting() : target(0) {}
    ResourceCollecting(int target_,
                       ResourseManager::STRATEGY_TYPE strategy_, int target_id_)
        : target(target_), strategy(strategy_), current(0), target_id(target_id_) {}
    ResourceCollecting(const ResourceCollecting &other) {
      current = other.current;
      target = other.target;
      active = other.active;
      strategy = other.strategy;
    }
    ResourceCollecting(ResourceCollecting &&other) {
      current = other.current;
      target = other.target;
      active = other.active;
      strategy = other.strategy;
      other.current = 0;
      other.target = 0;
      other.active = 0;
      other.strategy = ResourseManager::STRATEGY_TYPE::NONE;
    }
    bool start_collecting() {
      if (current >= target) return false;
      active = true;
      return true;
    }
  };
  std::queue<ResourceCollecting> rc;

 private:
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
      std::make_pair(STATUS, std::string("STATUS")),
      std::make_pair(GOTWORK, std::string("GOTWORK"))
    };

 public:
  static ResponseHandler& getInstance() {
    static ResponseHandler instance;
    return instance;
  }
  bool is_collecting_now() { return rc.size() > 0; }
  int read_response();
  bool _resource_collecting(int target,
                            ResourseManager::STRATEGY_TYPE strategy_) {}
  
  int solve_transfer_amount(ResourceCollecting rc, int amount);
  void set_resource_collecting(int target_, enum ResourseManager::STRATEGY_TYPE strategy_, int target_id_);
  int append_resource_collecting(int amount_);
  void solve_worker_fate(ResourseManager::STRATEGY_TYPE strategy, int id);
};

#endif  // UTIL_ZMQ_HELPERS_RESPONSE_HANDLER