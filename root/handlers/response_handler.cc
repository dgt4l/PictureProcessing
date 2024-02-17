#include "response_handler.h"

const std::string MESSAGE_PREFIX = "[ResponseHandler] ";

int ResponseHandler::read_response() {
  std::string response = s_recv(puller, ZMQ_DONTWAIT);
  if (response.length() > 0) {
    std::vector<std::string> args = auto_tokenize(response);
    std::cout << MESSAGE_PREFIX << "Recieved response: " << response
              << std::endl;
    switch (auto_hash_item(response, hasher)) {
      case ResponseHandler::RESPONSE_CODES::UNKNOWN: {
        break;
      }
      case ResponseHandler::RESPONSE_CODES::IAMALIVE: {
        worker_map.append_worker(stoi(args.at(1)), stoi(args.at(2)), 0, 0);
        break;
      }
      case ResponseHandler::RESPONSE_CODES::DEAD: {
        rm.return_threads(stoi(args.at(1)));
        worker_map.remove_worker(stoi(args.at(1)));
        break;
      }
      case ResponseHandler::RESPONSE_CODES::COMPLETE: {
        /*
            Responser::COMPLETE
            Возвращается, когда воркер успешно завершает свою задачу.
            Содержит в себе свой [id] и [output_path].
        */
        std::cout << "Worker 1 completed succesfully: " << args.at(2) << std::endl;
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
        if (is_collecting_now) {
          int gray_amount = stoi(args.at(2)) - worker_map.get_hard_limit(stoi(args.at(1)));
          std::cout << MESSAGE_PREFIX << "Recieved " << stoi(args.at(2)) << ", returning: " << std::to_string(gray_amount) << std::endl;
          append_resource_collecting(gray_amount);
          CommandDispatcher::getInstance().dispatch_command(
              "EXEC DELEGATE " + args.at(1) + " " + std::to_string(gray_amount));
        } else {
          // * return resource (we do not need them anymore)
          CommandDispatcher::getInstance().dispatch_command(
              "EXEC DELEGATE " + args.at(1) + " " + args.at(2));
        }
        break;
      }

      case ResponseHandler::RESPONSE_CODES::STATUS: {
        break;
      }
    }
  }
  return 1;
}

int ResponseHandler::solve_transfer_amount(ResourceCollecting rc, int amount) {
  if (!(is_collecting_now && rc.active)) return 0;
  switch (rc.strategy) {
    case ResourseManager::DESOLATE: {
      int amount_left = rc.target - rc.current;
      if (amount < amount_left) {
        // * get everything worker can offer
        rc.current += amount;
        return 0;
      }
      // * get only needed amount
      rc.active = is_collecting_now = false;
      return amount - amount_left;
    }

    case ResourseManager::DIVIDE: {
      return 0;
    }
  }
}

bool ResponseHandler::set_resource_collecting(int target_, enum ResourseManager::STRATEGY_TYPE strategy_) {
  if (is_collecting_now) return false;
  is_collecting_now = true;
  rc = ResourceCollecting(target_, strategy_);
  return rc.start_collecting();
}

/**
 * @brief 
 * 
 * @param amount 
 * @return int amount to return to the worker
 */
int ResponseHandler::append_resource_collecting(int amount_) {
  if (!is_collecting_now) return amount_;
  int amount_left = rc.target - rc.current;
  if (amount_left > amount_) {
    rc.current += amount_;
    return 0;
  }
  rc.current = rc.target;
  rc.active = false;
  is_collecting_now = false;
  return amount_ - amount_left;
}
