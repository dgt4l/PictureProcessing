#pragma once

#include <cppzmq/zmq.hpp>

class Responser{
    private:
    zmq::socket_t s;
    public:
        Responser();
        void send_response();
        void listen_request();
        enum class RESPONSE_TYPE {
            IAMALIVE, 
            DEAD, 
            COMPLETE, 
            BUSY, 
            TOOSWEET, 
            SHORTAGE, 
            TRANSFER, 
            STATUS
        };
};

