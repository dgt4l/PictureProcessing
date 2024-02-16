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
  int balance;
  // * maximum threads capacity for swarm
  const int capacity;

 public:
  enum STRATEGY_TYPE {
    SURRENDER,
    DESOLATE,
    DIVIDE,
  };  // * another could be implemented
  ResourseManager(const int capacity_)
      : balance(capacity_), capacity(capacity_) {}


  void common_worker_launch(std::string path_, int width_, int height_);

  static int calculate_resources(int width_, int height_);

  bool delegate_resources(int amount_, int id);
  bool solve_resource_shortage(int amount_, int id);

  STRATEGY_TYPE decide_strategy() { return SURRENDER; }

  static void slow_solve_thread(int amount_, int id);

  void return_threads(int id);


  // int solve_thread_to_image(Image& image, int id);

  // int return_size_image(Image& image);
};

#endif
