#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <iostream>
#include <thread>
#include <string>

// #include "../handlers/command_dispatcher.h"
// #include "../handlers/response_handler.h"

#include "worker_map.h"

class ResourseManager {
 private:
  // * how much threads available for new workers
  // * maximum threads capacity for swarm
  int capacity;
  int balance;

 public:
  enum STRATEGY_TYPE {
    NONE, // * everythin is OK, no need to apply strategy
    SURRENDER, // * server cannot afford this much resources
    DESOLATE, // * server is pulling all gray resources
    DIVIDE, // * applies dividing strategy
    QUENED // * server doesnt have this much resources available now, but retries if possible later
  };  // * another could be implemented
  ResourseManager() : balance(0), capacity(0) {}
  ResourseManager(int capacity_)
      : balance(capacity_), capacity(capacity_) {}
  
  void set_capacity(int capacity_) {capacity = capacity_; balance = capacity;}

  void modify_balance(int amount_) {balance += amount_;}

  int calculate_balance_space();
  int calculate_hard_limit(int width_, int height_);

  STRATEGY_TYPE delegate_resources(int limit, int hard_limit, int id);

  STRATEGY_TYPE decide_strategy(int amount_);

  static void slow_solve_thread(int amount_, int id, bool &status);

  void return_threads(int id);


  // int solve_thread_to_image(Image& image, int id);

  // int return_size_image(Image& image);
};

#endif
