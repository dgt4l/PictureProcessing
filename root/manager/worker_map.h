#ifndef ROOT_HANDLERS_WORKER_MAP_H
#define ROOT_HANDLERS_WORKER_MAP_H

#include <iostream>
#include <map>
#include <queue>

class WorkerMapElem {
    public:
    int pid;
    int limit;
    int min_limit;
    bool is_working = false;
  WorkerMapElem(int pid_, int limit_, int min_limit_)
      : pid(pid_), limit(limit_), min_limit(min_limit_) {}
};

class WaitingQueue {
  public:
   std::queue<std::pair<int, WorkerMapElem>> waiting_queue;
   WaitingQueue(){};
   void append_worker(WorkerMapElem wme, int id);
   int pop_worker();
   bool is_empty();
   int size();
};

class WorkerMap {
 public:
  std::map<int, WorkerMapElem> worker_map;
  WorkerMap(){};
  void append_worker(int id, int pid, int limit, int min_limit);
  int count_workers();
  bool is_reserved(int id);
  int get_pid(int id);
  int get_limit(int id);
  int get_hard_limit(int id);
  void remove_worker(int id);
  void set_is_working(int id);
  bool is_gotwork(int id);
  void set_limit(int id, int limit);
  void set_hard_limit(int id, int hard_limit_);
  int gray_resources();
  WorkerMapElem get_worker_by_id(int id);
};

extern WorkerMap worker_map;

extern WaitingQueue waiting_queue;

// void append_worker(int id, int pid, int limit);
// int count_workers();
// bool is_reserved(int id);
// int get_pid(int id);
// int get_limit(int id);
// void remove_worker(int id);
// void set_limit(int id, int limit);

#endif  // !ROOT_HANDLERS_WORKER_MAP_H
