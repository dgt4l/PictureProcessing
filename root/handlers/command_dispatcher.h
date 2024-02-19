#ifndef UTIL_ZMQ_HELPERS_COMMAND_DISPATCHER
#define UTIL_ZMQ_HELPERS_COMMAND_DISPATCHER

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <thread>
#include <zmq.hpp>

#include "../lib/cppzmq/zmq_helpers.hpp"
#include "../manager/worker_map.h"
#include "../utils/auto_handler.h"
#include "../utils/miscellaneous.h"

// #include "../manager/manager.h"

extern zmq::socket_t pusher;

class CommandDispatcher {
 private:
  CommandDispatcher() {}
  CommandDispatcher(const CommandDispatcher&);
  CommandDispatcher& operator=(CommandDispatcher&);
 

 public:
  enum CMD_CODES { UNKNOWN, EXIT, EXEC, CREATE };
  const std::list<std::pair<enum CMD_CODES, std::string>> hasher = {
      std::make_pair(UNKNOWN, std::string("UNKNOWN")),
      std::make_pair(EXIT, std::string("EXIT")),
      std::make_pair(EXEC, std::string("EXEC")),
      std::make_pair(CREATE, std::string("CREATE"))};
  static CommandDispatcher& getInstance() {
    static CommandDispatcher instance;
    return instance;
  }
  int dispatch_command(std::string cmd);
  int dispatch_command();
};

void init_worker_subprocess(std::string id);

#endif  // UTIL_ZMQ_HELPERS