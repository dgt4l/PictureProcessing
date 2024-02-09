#ifndef UTIL_ZMQ_HELPERS_RESPONSE_DISPATCHER
#define UTIL_ZMQ_HELPERS_RESPONSE_DISPATCHER

#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>
#include "../../lib/cppzmq/zmq_helpers.hpp"
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <utility>
#include "../root/handlers/worker_map.h"
#include "../../utils/auto_handler.h"

extern zmq::socket_t pusher;

class ResponseDispatcher {
  private:
    ResponseDispatcher() {}
    ResponseDispatcher( const ResponseDispatcher&);
    ResponseDispatcher& operator=(ResponseDispatcher&);
  public:
    static ResponseDispatcher& getInstance() {
      static ResponseDispatcher instance;
      return instance;
    }
    std::stringstream resps;
    int dispatch_response();
};

#endif  // UTIL_ZMQ_HELPERS