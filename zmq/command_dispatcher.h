#ifndef UTIL_ZMQ_HELPERS
#define UTIL_ZMQ_HELPERS

#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>
#include "../lib/cppzmq/zmq_helpers.hpp"
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>

const std::string SERVER_PUSHER_SOCKET_PATTERN = "tcp://127.0.0.1:5555";
const std::string SERVER_PULLER_SOCKET_PATTERN = "tcp://127.0.0.1:5556";

class CommandDispatcher{
  private:
    enum CMD_CODES { EXIT = -1, UNKNOWN, EXEC, CREATE };
    CommandDispatcher() {}
    CommandDispatcher( const CommandDispatcher&);
    CommandDispatcher& operator=(CommandDispatcher&);
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