#include "manager.h"


const std::string MESSAGE_PREFIX = "\e[0;32m[ResourceManager]\e[0m\t\t ";

int ResourseManager::calculate_balance_space() {
  return 1;
}

int ResourseManager::calculate_hard_limit(int width_, int height_) {
  return 1;
}


ResourseManager::STRATEGY_TYPE ResourseManager::delegate_resources(int limit, int hard_limit, int id) {
  worker_map.set_hard_limit(id, hard_limit);
  int actual_limit = std::max(limit, hard_limit);
  if (balance >= actual_limit) {
    balance -= actual_limit;
    worker_map.set_limit(id, actual_limit);
    std::cout << MESSAGE_PREFIX << "Resources delegated, balance: " <<
                std::to_string(balance + actual_limit) + "->" + std::to_string(balance) << std::endl;
    return NONE;
  }
  std::cout << MESSAGE_PREFIX << "Resources couldnot be delegated, balance: " <<
                std::to_string(balance) << std::endl;
  return decide_strategy(hard_limit);
}

ResourseManager::STRATEGY_TYPE ResourseManager::decide_strategy(int amount_) {
  int gray_amount = worker_map.gray_resources();
  std::cout << MESSAGE_PREFIX <<"Trying to allocate: " << amount_ << "/" << gray_amount << std::endl;
  if (capacity < amount_) return SURRENDER;
  if (gray_amount < amount_) return QUENED; // * but really is QUENED, not implemented
  return DESOLATE;
}

void ResourseManager::return_threads(int id) {
  int buff = worker_map.get_limit(id);
  worker_map.set_limit(id, 0);
  worker_map.set_hard_limit(id, 0);
  // * set worker threads to zero
  balance += buff;  // * here goes worker balance
}
