#include "manager.h"


static int calculate_resources(int width_, int height_) {
  return 5;
}

static bool delegate_resources(int amount_, int id) {
  if (balance >= amount_) {
    balance -= amount_;
    worker_map.set_limit(id, amount_);
    return true;
  }
  return solve_resource_shortage(amount_, id);
}

static bool solve_resource_shortage(int amount_, int id) {
  std::thread ss_thread(ResourseManager::slow_solve_thread, amount_, id);
  return false;
}

void ResourseManager::slow_solve_thread(int amount_, int id) {
  // * do fucking slow solve
  // * pretty tough to implement but essential
  switch (DESOLATE) {
      // * ...
    case SURRENDER: {
      // * notify client about failed attempt to allocate resources
      break;
    }
    case DESOLATE: {
      // ResponseHandler::getInstance().set_resource_collecting(amount_, DESOLATE);
      // CommandDispatcher::getInstance().dispatch_command("EXEC DESOLATE");
      break;
    }
    case DIVIDE: {
      // int buffer = 0;
      // int subtract = amount_ / worker_map.count_workers();
      // if (subtract == 0) { subtract = 1; }
      // for (const auto& [key, value] : worker_map){
      //   if (buffer < amount_){
      //     if (it->second.limit - it->second.min_limit >= subtract){
      //       it->second.limit -= subtract;
      //       buffer += subtract;
      //     } else {
      //       it->second.limit -= it->second.limit - it->second.min_limit;
      //       buffer += it->second.limit - it->second.min_limit;
      //     }
      //   } else {
      //     break;
      //   }
      // }
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