#include "worker_map.h"

const std::string MESSAGE_PREFIX = "[WorkerMap]";

void WorkerMap::append_worker(int id, int pid_, int limit_, int min_limit_) {
  std::cout << MESSAGE_PREFIX << " New worker added: [" << id << ":" << pid_
            << ":" << limit_ << ":" << min_limit_ << "]" << std::endl;
  WorkerMapElem worker(pid_, limit_, min_limit_);
  worker_map.insert({id, worker});
}

int WorkerMap::count_workers() { return worker_map.size(); }

int WorkerMap::get_pid(int id) { return worker_map.at(id).pid; }

int WorkerMap::get_limit(int id) { return worker_map.at(id).limit; }

bool WorkerMap::is_reserved(int id) {
  auto it = worker_map.find(id);
  if (it == worker_map.end()) {
    return false;
  };
  return true;
}

void WorkerMap::remove_worker(int id) {
  std::cout << MESSAGE_PREFIX << " Worker with id: [" << id << "]"
            << " has been removed" << std::endl;
  worker_map.erase(id);
}

void WorkerMap::set_limit(int id, int limit_) {
  worker_map.at(id).limit = limit_;
}