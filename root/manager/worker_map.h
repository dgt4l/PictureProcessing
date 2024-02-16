#ifndef ROOT_HANDLERS_WORKER_MAP_H
#define ROOT_HANDLERS_WORKER_MAP_H

#include <iostream>
#include <map>

extern WorkerMap worker_map;

class WorkerMapElem {
 public:
  int pid;
  int limit;
  int min_limit;

  WorkerMapElem(int pid_, int limit_, int min_limit_)
      : pid(pid_), limit(limit_), min_limit(min_limit_) {}
};

class WorkerMap {
 private:
  std::map<int, WorkerMapElem> worker_map;

 public:
  WorkerMap(){};
  void append_worker(int id, int pid, int limit, int min_limit);
  int count_workers() { return worker_map.size(); }
  bool is_reserved(int id);
  int get_pid(int id);
  int get_limit(int id);
  void remove_worker(int id);
  void set_limit(int id, int limit);
};

// void append_worker(int id, int pid, int limit);
// int count_workers();
// bool is_reserved(int id);
// int get_pid(int id);
// int get_limit(int id);
// void remove_worker(int id);
// void set_limit(int id, int limit);

#endif  // !ROOT_HANDLERS_WORKER_MAP_H
