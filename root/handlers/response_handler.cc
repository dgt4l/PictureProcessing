#include "response_handler.h"

void wait_collection_end_thread(int limit, int hard_limit, int id);

const std::string MESSAGE_PREFIX = "\e[0;32m[ResponseHandler]\e[0m\t\t ";

int ResponseHandler::read_response() {
  std::string response = s_recv(puller, ZMQ_DONTWAIT);
  if (response.length() > 0) {
    std::vector<std::string> args = auto_tokenize(response);
    std::cout << MESSAGE_PREFIX << "Recieved response: \e[0;95m" << response
              << "\e[0m" << std::endl;
    switch (auto_hash_item(response, hasher)) {
      case ResponseHandler::RESPONSE_CODES::UNKNOWN: {
        break;
      }
      
      case ResponseHandler::RESPONSE_CODES::IAMALIVE: {
        worker_map.append_worker(stoi(args.at(1)), stoi(args.at(2)), 0, 0);
        break;
      }

      case ResponseHandler::RESPONSE_CODES::DEAD: {
        int id = stoi(args.at(1));
        rm.return_threads(id);
        worker_map.remove_worker(id);
        for (int i = 0; i < wq.size(); ++i) {
          int candidate = wq.pop_worker();
          std::cout << MESSAGE_PREFIX << "Trying allocate resources for quened worker: " << candidate << std::endl;
          int hard_limit = worker_map.get_hard_limit(candidate);
          int limit = std::max(rm.calculate_balance_space(), hard_limit);
          solve_worker_fate(rm.delegate_resources(limit, hard_limit, candidate), candidate);
        }
        break;
      }

      case ResponseHandler::RESPONSE_CODES::COMPLETE: {
        /*
            Responser::COMPLETE
            Возвращается, когда воркер успешно завершает свою задачу.
            Содержит в себе свой [id] и [output_path].
        */
        std::cout << MESSAGE_PREFIX << "\e[0;33mWorker [" + args.at(1) + "] completed succesfully: " << args.at(2) << "\e[0m" << std::endl;
        CommandDispatcher::getInstance().dispatch_command("EXEC FREE " +
                                                          args.at(1));
        break;
      }
      case ResponseHandler::RESPONSE_CODES::BUSY: {
        // handle_error();
        break;
      }

      case ResponseHandler::RESPONSE_CODES::TOOSWEET: {
        // handle_error();
        break;
      }

      case ResponseHandler::RESPONSE_CODES::SHORTAGE: {
        break;
      }

      case ResponseHandler::RESPONSE_CODES::TRANSFER: {
        int amount = stoi(args.at(2)), id = stoi(args.at(1));
        if (rc.size() > 0 && amount != 0) {
          ResourceCollecting current_rc = rc.front();
          if (current_rc.target_id != id) {
            int hard_limit = worker_map.get_hard_limit(stoi(args.at(1)));
            int gray_amount = std::max(0, stoi(args.at(2)) - hard_limit);
            int new_gray_amount = append_resource_collecting(gray_amount);
            std::cout << MESSAGE_PREFIX << "Recieved " << stoi(args.at(2)) << ", returning: " << std::to_string(new_gray_amount) << std::endl;
            if (!is_collecting_now()) {
              std::cout << MESSAGE_PREFIX << "Collection completed:" + std::to_string(current_rc.current) + "/" + std::to_string(current_rc.target) << std::endl;
              rm.modify_balance(current_rc.current);
            } else {
              std::cout << MESSAGE_PREFIX << "Still not reached, balance: " + std::to_string(current_rc.current) + "/" + std::to_string(current_rc.target) << std::endl;
            }
            CommandDispatcher::getInstance().dispatch_command(
                "EXEC DELEGATE " + args.at(1) + " " + std::to_string(hard_limit + new_gray_amount));
          } else {
            std::cout << MESSAGE_PREFIX << "Ignored desolate for id: " << id << std::endl;
          }
        } else {
          // * return resource (we do not need them anymore)
          CommandDispatcher::getInstance().dispatch_command(
              "EXEC DELEGATE " + args.at(1) + " " + std::to_string(worker_map.get_limit(id)));
        }
        break;
      }

      case ResponseHandler::RESPONSE_CODES::STATUS: {
        break;
      }
      case ResponseHandler::RESPONSE_CODES::GOTWORK: {
        int id = stoi(args.at(1));
        worker_map.set_is_working(id);
        break;
      }
    }
  }
  return 1;
}

void read_response_thread() {
  
}

int ResponseHandler::solve_transfer_amount(ResourceCollecting rc, int amount) {
  if (!(is_collecting_now() && rc.active)) return 0;
  switch (rc.strategy) {
    case ResourseManager::DESOLATE: {
      int amount_left = rc.target - rc.current;
      if (amount < amount_left) {
        // * get everything worker can offer
        rc.current += amount;
        return 0;
      }
      // * get only needed amount
      return amount - amount_left;
    }

    case ResourseManager::DIVIDE: {
      return 0;
    }
  }
}

void ResponseHandler::set_resource_collecting(int target_, enum ResourseManager::STRATEGY_TYPE strategy_, int target_id_) {
  ResourceCollecting new_rc(target_, strategy_, target_id_);
  rc.push(new_rc);
  // std::cout << MESSAGE_PREFIX << "Start collecting resources: " << rc.current << "/" << rc.target << "[" << target_id_ << "]" << std::endl;
}

/**
 * @brief 
 * 
 * @param amount 
 * @return int amount to return to the worker
 */
int ResponseHandler::append_resource_collecting(int amount_) {
  std::cout << "input:" << amount_ << std::endl;
  ResourceCollecting current_rc = rc.front();
  int amount_left = current_rc.target - current_rc.current;
  if (amount_left > amount_) {
    current_rc.current += amount_;
    return 0;
  }
  current_rc.current = current_rc.target;
  rc.pop();
  return amount_ - amount_left;
}

void ResponseHandler::solve_worker_fate(ResourseManager::STRATEGY_TYPE strategy, int id) {
  WorkerMapElem worker = worker_map.get_worker_by_id(id);
  int limit = worker.limit, hard_limit = worker.min_limit;
  switch (strategy) {
    case ResourseManager::STRATEGY_TYPE::NONE: {
      CommandDispatcher::getInstance().dispatch_command("EXEC DELEGATE " + std::to_string(id) + " " + std::to_string(limit));
      break;
    }
    
    case ResourseManager::STRATEGY_TYPE::DESOLATE: {
      // * simply casts desolate directive to all workers
      ResponseHandler::getInstance().set_resource_collecting(hard_limit, strategy, id);
      CommandDispatcher::getInstance().dispatch_command("EXEC DESOLATE");
      std::thread wcet(wait_collection_end_thread, limit, hard_limit, id);
      wcet.detach();
      break;
    }

    case ResourseManager::STRATEGY_TYPE::SURRENDER: {
      // * return by inner channels about failed attempt to allocate resources
      std::cout << MESSAGE_PREFIX << "Gave up allocating this much resources: " << std::to_string(limit) << std::endl;
      break;
    }

    case ResourseManager::STRATEGY_TYPE::QUENED: {
      WorkerMapElem worker = worker_map.get_worker_by_id(id);
      wq.append_worker(worker, id);
      std::cout << MESSAGE_PREFIX << "Quening task associated with worker id: [" + std::to_string(id) + "]" << std::endl;
      break;
    }

    case ResourseManager::STRATEGY_TYPE::DIVIDE: {
      // * do divide tricks
      break;
    }
  }
}

void wait_collection_end_thread(int limit, int hard_limit, int id) {
  while (ResponseHandler::getInstance().is_collecting_now() && ResponseHandler::getInstance().rc.front().target_id == id)
        ; // * wait till resource collecting ends
  std::cout << MESSAGE_PREFIX << "Resources collection ends for id: " << std::to_string(id) << std::endl;
  ResourseManager::STRATEGY_TYPE strategy = rm.delegate_resources(limit, hard_limit, id);
  CommandDispatcher::getInstance().dispatch_command("EXEC DELEGATE " + std::to_string(id) + " " + std::to_string(std::max(limit, hard_limit)));
}
