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
#include "../utils/miscellaneous.h"
#include <utility>
#include <thread>
#include <sys/wait.h>

struct Task {
    enum Filter { NONE, BLUR, CLARITY, EMBOSSING, BORDERS, UPSCALING, NEGATIVE };
    std::string input_path;
    Filter filter;
    const std::list<std::pair<enum Filter, std::string>> filter_hasher = 
       {
        std::make_pair(NONE, std::string("NONE")),
        std::make_pair(BLUR, std::string("BLUR")),
        std::make_pair(CLARITY, std::string("CLARITY")),
        std::make_pair(EMBOSSING, std::string("EMBOSSING")),
        std::make_pair(BORDERS, std::string("BORDERS")),
        std::make_pair(UPSCALING, std::string("UPSCALING")),
        std::make_pair(NEGATIVE, std::string("NEGATIVE"))
       };
};

class Worker {
    public:
      int id;
      enum Status { IDLE, READY, WORKING, PAUSED, COMPLETE };
      Task task = {"", Task::Filter::NONE};
      const std::list<std::pair<enum Status, std::string>> status_hasher = 
       {
        std::make_pair(IDLE, std::string("IDLE")),
        std::make_pair(READY, std::string("READY")),
        std::make_pair(WORKING, std::string("WORKING")),
        std::make_pair(PAUSED, std::string("PAUSED")),
        std::make_pair(COMPLETE, std::string("COMPLETE"))
       };
      int limit;
      int hard_limit;
      Status s;
      Worker() {}
      Worker(enum Status s_, int id_) : s(s_), id(id_), limit(-1) {}
      Worker& operator=(const Worker& right) {
        id = right.id;
        task.input_path = right.task.input_path;
        task.filter = right.task.filter;
        limit = right.limit;
        s = right.s;
        return *this;
      }
      void set_task(std::string input_path_, Task::Filter filter_) {
        task.input_path = input_path_;
        task.filter = filter_;
      }
};

extern zmq::socket_t puller;
extern Worker worker;

class RequestHandler {
  private:
    enum REQUEST_CODES { UNKNOWN, TERMINATE, TASK, DELEGATE, STATUS, FREE, DESOLATE };
    RequestHandler() {}
    RequestHandler( const RequestHandler&);
    RequestHandler& operator=(RequestHandler&);
    const std::list<std::pair<enum REQUEST_CODES, std::string>> hasher = 
       {
        std::make_pair(UNKNOWN, std::string("UNKNOWN")),
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
    int read_request(std::string request);
};

void working_thread();
int working_iteration(int channel);

#endif // !CLIENT_HANDLERS_REQUEST_HANDLER_H