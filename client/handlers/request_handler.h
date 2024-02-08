#ifndef CLIENT_HANDLERS_REQUEST_HANDLER_H
#define CLIENT_HANDLERS_REQUEST_HANDLER_H


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


extern zmq::socket_t puller;

class RequestHandler {
  private:
    enum REQUEST_CODES { UNKNOWN, CREATE, TERMINATE, TASK, DELEGATE, STATUS, FREE, DESOLATE };
    RequestHandler() {}
    RequestHandler( const RequestHandler&);
    RequestHandler& operator=(RequestHandler&);
    enum REQUEST_CODES solve_request(const std::string request);
    
  public:
    static RequestHandler& getInstance() {
      static RequestHandler instance;
      return instance;
    }
    int read_request();
};

#endif // !CLIENT_HANDLERS_REQUEST_HANDLER_H