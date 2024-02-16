#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <iostream>

#include "worker_map.h"

class ResourseManager {
 private:
  // * how much threads available for new workers
  int balance;
  // * maximum threads capacity for swarm
  const int capacity;

 public:
  enum STRATEGY_TYPE {
    SURRENDER,
    DESOLATE,
    DIVIDE
  };  // * another could be implemented
  ResourseManager(const int capacity_)
      : balance(capacity_), capacity(capacity_) {}

  bool delegate_resources(int amount_, int id);
  bool solve_resource_shortage(int amount_, int id) {
    // * try fast solve (if possible)
    // * if successful return True
    // * else do slow solve with preferred strategy in separated thread
    // * after slow solve thread launch notify client about it with return False
  }

  STRATEGY_TYPE decide_strategy() { return SURRENDER; }

  void slow_solve_thread();

  void return_threads(int id);

  int solve_thread_to_image(Image& image, int id);

  int return_size_image(Image& image);
};

#endif
