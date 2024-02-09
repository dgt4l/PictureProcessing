#ifndef CLIENT_WORKER_H
#define CLIENT_WORKER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <thread>

#include "../lib/cppzmq/zmq_helpers.hpp"
#include "handlers/response_dispatcher.h"


struct Task {
    enum class Filter { NONE, BLUR, CLARITY, EMBOSSING, BORDERS, UPSCALING, NEGATIVE };
    std::string input_path;
    Filter filter;
};

class Worker {
    private:
        int id;
    public:
        enum class Status { IDLE, READY, WORKING, PAUSED, COMPLETE };
        Task task;
        int limit;
        Status s;
        Worker(enum class Status s_, int id_) {
            s = s_;
            id = id_;
            limit = -1;
        }
};

class WorkerReciever {
  private:
    WorkerReciever() {}
    WorkerReciever( const WorkerReciever&);
    WorkerReciever& operator=(WorkerReciever&);
  public:
    static WorkerReciever& getInstance() {
      static WorkerReciever instance;
      return instance;
    }
    int read_command();
};
#endif // !CLIENT_WORKER_H