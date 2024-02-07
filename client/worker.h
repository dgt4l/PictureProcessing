#ifndef CLIENT_WORKER_H
#define CLIENT_WORKER_H

class Worker {
    private:
        enum class Status { IDLE, READY, WORKING, PAUSED, COMPLETE };
        int id;
    public:
        Worker(enum class Status_, int id_) (Status: Status_, id: id_)
};

#endif // !CLIENT_WORKER_H