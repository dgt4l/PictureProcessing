#ifndef UTIL_ZMQ_HELPERS_PUSHER_HANDLER
#define UTIL_ZMQ_HELPERS_PUSHER_HANDLER

#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>
#include "../lib/cppzmq/zmq_helpers.hpp"
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>

extern zmq::socket_t pusher;

class PusherHandler {
  public:
    void dispatch_request(std::string msg) {
      pusher.send(zmq::buffer(msg), zmq::send_flags::dontwait);
    }
};

#endif  // UTIL_ZMQ_PUSHER_HANDLER