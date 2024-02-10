#ifndef UTIL_ZMQ_HELPERS_RESPONSE_HANDLER
#define UTIL_ZMQ_HELPERS_RESPONSE_HANDLER

#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>
#include "../../lib/cppzmq/zmq_helpers.hpp"
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>
#include "worker_map.h"
#include "../utils/auto_handler.h"
#include "../utils/miscellaneous.h"
#include <utility>

extern zmq::socket_t puller;

class ResponseHandler {
  private:
    enum RESPONSE_CODES { UNKNOWN, IAMALIVE, DEAD, COMPLETE, BUSY, TOOSWEET, SHORTAGE, TRANSFER, STATUS };
    ResponseHandler() {}
    ResponseHandler( const ResponseHandler&);
    ResponseHandler& operator=(ResponseHandler&);
    const std::list<std::pair<enum RESPONSE_CODES, std::string>> hasher = 
       {
        std::make_pair(UNKNOWN, std::string("UNKNOWN")),
        std::make_pair(IAMALIVE, std::string("IAMALIVE")),
        std::make_pair(DEAD, std::string("DEAD")),
        std::make_pair(COMPLETE, std::string("COMPLETE")),
        std::make_pair(BUSY, std::string("BUSY")),
        std::make_pair(TOOSWEET, std::string("TOOSWEET")),
        std::make_pair(SHORTAGE, std::string("SHORTAGE")),
        std::make_pair(TRANSFER, std::string("TRANSFER")),
        std::make_pair(STATUS, std::string("STATUS"))
       };
  public:
    static ResponseHandler& getInstance() {
      static ResponseHandler instance;
      return instance;
    }
    int read_response();
};

#endif  // UTIL_ZMQ_HELPERS_RESPONSE_HANDLER