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


extern zmq::socket_t puller;

class ResponseHandler {
  private:
    enum RESPONSE_CODES { UNKNOWN, IAMALIVE, DEAD, COMPLETE, BUSY, TOOSWEET, SHORTAGE, TRANSFER, STATUS };
    ResponseHandler() {}
    ResponseHandler( const ResponseHandler&);
    ResponseHandler& operator=(ResponseHandler&);
    enum RESPONSE_CODES solve_response(const std::string response);
  public:
    static ResponseHandler& getInstance() {
      static ResponseHandler instance;
      return instance;
    }
    int read_response();
};

#endif  // UTIL_ZMQ_HELPERS_RESPONSE_HANDLER