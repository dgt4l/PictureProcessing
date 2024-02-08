#include "worker_map.h"

void append_worker(int id, int pid){
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

void remove_worker(int id){
    worker_map.erase(id);
}