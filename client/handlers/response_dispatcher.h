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
#include "../../root/manager/worker_map.h"
#include "../../utils/auto_handler.h"

extern zmq::socket_t pusher;

class Response {
  public:
    enum RESPONSE_CODES { UNKNOWN, IAMALIVE, DEAD, COMPLETE, BUSY, TOOSWEET, SHORTAGE, TRANSFER, STATUS, GOTWORK };
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
          std::make_pair(STATUS, std::string("STATUS")),
          std::make_pair(GOTWORK, std::string("GOTWORK"))
        };
    virtual std::string to_string() {
      return "";
    }
    int dispatch_response() {
        pusher.send(zmq::buffer((*this).to_string()), zmq::send_flags::dontwait);
    }
};

class SimpleResponse : public Response {
  private:
    RESPONSE_CODES response;
    std::string id;
  public:
    SimpleResponse(Response::RESPONSE_CODES response_, std::string id_) : response(response_), id(id_) {}
    std::string to_string() {
      return auto_dehash_item(response, hasher) + " " + id;
    }
};

class ComplexResponse : public Response {
  private:
    RESPONSE_CODES response;
    std::vector<std::string> response_params;
  public:
    ComplexResponse(RESPONSE_CODES response_, std::vector<std::string> response_params_) : response(response_), response_params(response_params_) {}
    std::string to_string() {
        std::string params;
        for (const auto &rp : response_params) params += rp + " ";
        return auto_dehash_item(response, hasher) + " " + params;
    }
};



#endif  // UTIL_ZMQ_HELPERS