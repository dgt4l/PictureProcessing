#ifndef ROOT_SERVER_H
#define ROOT_SERVER_H

#include "zmq.hpp"
#include <thread>
#include <string>
#include "manager/manager.h"

extern zmq::context_t ctx;
extern zmq::socket_t pusher;
extern zmq::socket_t puller;

#endif // ! ROOT_SERVER_H