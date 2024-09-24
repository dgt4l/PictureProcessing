#include "worker_map.h"

const std::string MESSAGE_PREFIX = "\e[0;32m[WorkerMap]\e[0m\t\t\t ";

void WorkerMap::append_worker(int id, int pid_, int limit_, int min_limit_) {
  std::cout << MESSAGE_PREFIX << "\e[0;33mNew worker added: [" << id << ":" << pid_
            << ":" << limit_ << ":" << min_limit_ << "]\e[0m" << std::endl;
  WorkerMapElem worker(pid_, limit_, min_limit_);
  worker_map.insert({id, worker});
}

int WorkerMap::count_workers() { return worker_map.size(); }

int WorkerMap::get_pid(int id) { return worker_map.at(id).pid; }

int WorkerMap::get_limit(int id) { return worker_map.at(id).limit; }

int WorkerMap::get_hard_limit(int id) { return worker_map.at(id).min_limit; }

bool WorkerMap::is_gotwork(int id) { return worker_map.at(id).is_working; }

bool WorkerMap::is_reserved(int id) {
  auto it = worker_map.find(id);
  if (it == worker_map.end()) {
    return false;
  };
  return true;
}

void WorkerMap::remove_worker(int id) {
  worker_map.erase(id);
  std::cout << MESSAGE_PREFIX << "\e[0;31mWorker with id: [" << id << "]"
            << " has been removed, current count: \e[0m" << count_workers() << std::endl;
}

void WorkerMap::set_limit(int id, int limit_) {
  worker_map.at(id).limit = limit_;
}

void WorkerMap::set_hard_limit(int id, int hard_limit_) {
  worker_map.at(id).min_limit = hard_limit_;
}

void WorkerMap::set_is_working(int id) {
  worker_map.at(id).is_working = true;
}

int WorkerMap::gray_resources(){
  int gray_buffer = 0;
  for(auto it = worker_map.begin(); it != worker_map.end(); ++it){
    gray_buffer += std::max(0, it->second.limit - it->second.min_limit);
  }
  return gray_buffer;
}

WorkerMapElem WorkerMap::get_worker_by_id(int id){
  try{
    WorkerMapElem wme = worker_map.at(id);
    return wme;
  }
  catch (const std::out_of_range&){
    std::cout << "In function {get_worker_by_id} Worker with id: " << id << "doesn`t exist" << std::endl;
  }
}

void WaitingQueue::append_worker(WorkerMapElem wme, int id){
  waiting_queue.push({id, wme});
}

int WaitingQueue::pop_worker(){
  int id_out;
  id_out = waiting_queue.front().first;
  waiting_queue.pop();
  return id_out;
}

int WaitingQueue::size(){ return waiting_queue.size(); }

bool WaitingQueue::is_empty(){ return waiting_queue.empty(); }

