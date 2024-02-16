#include "manager.h"

bool ResourseManager::delegate_resources(int amount_, int id) {
  if (balance >= amount_) {
    balance -= amount_;
    worker_map.set_limit(id, amount_);
    // * worker map associate with id specific amount
  } else {
    // ? no available resources for this task
    return solve_resource_shortage(amount_, id);
  }
  return true;
}

bool ResourseManager::solve_resource_shortage(int amount_, int id) {
  slow_solve_thread();
  return false;
}

void ResourseManager::slow_solve_thread() {
  // * do fucking slow solve
  // * pretty tough to implement but essential
  switch (decide_strategy()) {
      // * ...
    case SURRENDER: {
      // * notify client about failed attempt to allocate resources
      break;
    }
    case DESOLATE: {
      break;
    }
    case DIVIDE: {
      break;
    }
  }
}

void ResourseManager::return_threads(int id) {
  int buff = worker_map.get_limit(id);
  worker_map.set_limit(id, 0);
  // * set worker threads to zero
  balance += buff;  // * here goes worker balance
}

// int ResourseManager::solve_thread_to_image(Image& image, int id){
//     int image_size = return_size_image(image);

//     if(image_size <;600){
//         return 3;
//     } else if(image_size < 1000){
//         return 4;
//     } else {
//         return 5;
//     }
// }

// int ResourseManager::return_size_image(Image& image) {
//     int image_size = image.h * image.w;
//     return image_size;
// }