#ifndef UTIL_ZMQ_HELPERS_COMMAND_DISPATCHER
#define UTIL_ZMQ_HELPERS_COMMAND_DISPATCHER

#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>
#include "../lib/cppzmq/zmq_helpers.hpp"
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
#include "worker_map.h"

extern zmq::socket_t pusher;

class CommandDispatcher {
  private:
    CommandDispatcher() {}
    CommandDispatcher( const CommandDispatcher&);
    CommandDispatcher& operator=(CommandDispatcher&);
    enum CMD_CODES { EXIT = -1, UNKNOWN, EXEC, CREATE };
    enum CMD_CODES solve_command(const std::string cmd);
  public:
    static CommandDispatcher& getInstance() {
      static CommandDispatcher instance;
      return instance;
    }
    int dispatch_command();
};

void init_worker_subprocess(std::string id);


#endif  // UTIL_ZMQ_HELPERS