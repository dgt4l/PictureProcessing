#include "worker_map.h"

const std::string MESSAGE_PREFIX = "[WorkerMap]";

void append_worker(int id, int pid, int limit) {
    std::cout << MESSAGE_PREFIX << " New worker added: [" << id << ":" << pid << ":" << limit << "]" << std::endl; 
    worker_map.insert({id, std::make_pair(pid, limit)});
}

int count_workers() {
    return worker_map.size();
}

int get_pid(int id) {
    return worker_map.at(id).first;
}

int get_limit(int id) {
    return worker_map.at(id).second;
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

void set_limit(int id, int limit){
    worker_map.at(id).second = limit;
}