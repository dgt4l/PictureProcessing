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
#include "../utils/auto_handler.h"
#include "response_dispatcher.h"
#include <utility>

const std::string GLOBAL_ID = "-1";

struct Task {
    enum class Filter { NONE, BLUR, CLARITY, EMBOSSING, BORDERS, UPSCALING, NEGATIVE };
    std::string input_path;
    Filter filter;
};

class Worker {
    public:
      int id;
      enum class Status { IDLE, READY, WORKING, PAUSED, COMPLETE };
      Task task;
      int limit;
      Status s;
      Worker() {}
      Worker(enum class Status s_, int id_) : s(s_), id(id_), limit(-1) {}
};

extern zmq::socket_t puller;
extern Worker worker;

class RequestHandler {
  private:
    enum REQUEST_CODES { UNKNOWN, CREATE, TERMINATE, TASK, DELEGATE, STATUS, FREE, DESOLATE };
    RequestHandler() {}
    RequestHandler( const RequestHandler&);
    RequestHandler& operator=(RequestHandler&);
    const std::list<std::pair<enum REQUEST_CODES, std::string>> hasher = 
       {
        std::make_pair(UNKNOWN, std::string("UNKNOWN")),
        std::make_pair(CREATE, std::string("CREATE")),
        std::make_pair(TERMINATE, std::string("TERMINATE")),
        std::make_pair(TASK, std::string("TASK")),
        std::make_pair(DELEGATE, std::string("DELEGATE")),
        std::make_pair(STATUS, std::string("STATUS")),
        std::make_pair(FREE, std::string("FREE")),
        std::make_pair(DESOLATE, std::string("DESOLATE"))
       };
    
  public:
    static RequestHandler& getInstance() {
      static RequestHandler instance;
      return instance;
    }
    int read_request();
};

#endif // !CLIENT_HANDLERS_REQUEST_HANDLER_H