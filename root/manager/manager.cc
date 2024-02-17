#include "manager.h"


const std::string MESSAGE_PREFIX = "[ResourceManager] ";

int ResourseManager::calculate_limit(int width_, int height_) {
  std::cout << "Current worker count:" << worker_map.count_workers() << std::endl;
  if (worker_map.count_workers() == 1) return balance;
  return 5;
}

int ResourseManager::calculate_hard_limit(int width_, int height_) {
  return 5;
}


ResourseManager::STRATEGY_TYPE ResourseManager::delegate_resources(int limit, int hard_limit, int id) {
  if (balance >= limit) {
    balance -= limit;
    std::cout << MESSAGE_PREFIX << "Resources delegated, balance: " <<
                std::to_string(balance + limit) + "->" + std::to_string(balance) << std::endl;
    worker_map.set_limit(id, limit);
    worker_map.set_hard_limit(id, hard_limit);
    return NONE;
  }
  std::cout << MESSAGE_PREFIX << "Resources couldnot be delegated, balance: " <<
                std::to_string(balance) << std::endl;
  return decide_strategy(limit);
}

ResourseManager::STRATEGY_TYPE ResourseManager::decide_strategy(int amount_) {
  int gray_amount = worker_map.gray_resources();
  std::cout << "Trying to allocate: " << amount_ << "/" << gray_amount << std::endl;
  if (capacity < amount_) return SURRENDER;
  if (gray_amount < amount_) return SURRENDER; // * but really is QUENED, not implemented
  return DESOLATE;
}

void ResourseManager::return_threads(int id) {
  int buff = worker_map.get_limit(id);
  worker_map.set_limit(id, 0);
  worker_map.set_hard_limit(id, 0);
  // * set worker threads to zero
  balance += buff;  // * here goes worker balance
}
