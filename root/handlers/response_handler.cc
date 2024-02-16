#include "response_handler.h"

const std::string MESSAGE_PREFIX = "[ResponseHandler] ";

int ResponseHandler::read_response(const std::string response) {
  if (response.length() > 0) {
    std::vector<std::string> args = auto_tokenize(response);
    std::cout << MESSAGE_PREFIX << "Recieved response: " << response
              << std::endl;
    switch (auto_hash_item(response, hasher)) {
      case ResponseHandler::RESPONSE_CODES::UNKNOWN: {
        break;
      }
      case ResponseHandler::RESPONSE_CODES::IAMALIVE: {
        append_worker(stoi(args.at(1)), stoi(args.at(2)));
        break;
      }
      case ResponseHandler::RESPONSE_CODES::DEAD: {
        remove_worker(stoi(args.at(1)));
        break;
      }
      case ResponseHandler::RESPONSE_CODES::COMPLETE: {
        /*
            Responser::COMPLETE
            Возвращается, когда воркер успешно завершает свою задачу.
            Содержит в себе свой [id] и [output_path].
        */
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

int ResponseHandler::desolate_append_amount(ResourceCollecting rc, int amount) {
  if (!(is_collecting_now && rc.active)) return 0;
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
