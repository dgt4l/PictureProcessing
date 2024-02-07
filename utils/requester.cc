#include "requester.h"

Requester::Requester(zmq::socket_t s) 
 : s(std::move(s)) {}