#pragma once

#include <cppzmq/zmq.hpp>
#include "requester.h"

class Requester {
    private:
    zmq::socket_t s;
    public:
        Requester(zmq::socket_t);
        void send_request();
        void listen_response();
        enum class REQUEST_TYPE {
            CREATE, 
            TERMINATE, 
            TASK, 
            DELEGATE, 
            STATUS, 
            FREE, 
            DESOLATE
        };
};





