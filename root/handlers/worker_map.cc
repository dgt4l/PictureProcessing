#include "worker_map.h"

const std::string MESSAGE_PREFIX = "[WorkerMap]";

void append_worker(int id, int pid) {
    std::cout << MESSAGE_PREFIX << " New worker added: [" << id << ":" << pid << "]" << std::endl; 
    worker_map.insert({id, pid});
}

int count_workers() {
    return worker_map.size();
}

int get_pid(int id){
    return worker_map.at(id);
}

bool is_reserved(int id){
    auto it = worker_map.find(id);
    if(it == worker_map.end()){
        return false;
    };
    return true;
} 

void remove_worker(int id) {
    std::cout << MESSAGE_PREFIX << " Worker with id: [" << id << "]" <<" has been removed" << std::endl;
    worker_map.erase(id);
}
