#include "request_handler.h"

const std::string MESSAGE_PREFIX = "[RequestHandler] ";

enum RequestHandler::REQUEST_CODES RequestHandler::solve_request(const std::string request) {
  std::string head = request.substr(0, request.find(" "));
  std::cout << "[" << head << "]" << std::endl;
  if (!head.compare("CREATE")) return RequestHandler::REQUEST_CODES::CREATE;
  if (!head.compare("TERMINATE")) return RequestHandler::REQUEST_CODES::TERMINATE;
  if (!head.compare("TASK")) return RequestHandler::REQUEST_CODES::TASK;
  if (!head.compare("DELEGATE")) return RequestHandler::REQUEST_CODES::DELEGATE;
  if (!head.compare("FREE")) return RequestHandler::REQUEST_CODES::FREE;
  if (!head.compare("DESOLATE")) return RequestHandler::REQUEST_CODES::DESOLATE;
  if (!head.compare("STATUS")) return RequestHandler::REQUEST_CODES::STATUS;

  return RequestHandler::REQUEST_CODES::UNKNOWN;
}

int RequestHandler::read_request() {
    std::string request = s_recv(puller, ZMQ_DONTWAIT);
    if (request.length() > 0) {
        std::vector<std::string> args = auto_tokenize(request);
        std::cout << MESSAGE_PREFIX << "Recieved request: " << args.at(0) << std::endl;
        switch (solve_request(request)) {
            case RequestHandler::REQUEST_CODES::UNKNOWN: {
                break;
            }
            case RequestHandler::REQUEST_CODES::CREATE: {
                break;
            }
            case RequestHandler::REQUEST_CODES::TERMINATE: {
                break;
            }
            case RequestHandler::REQUEST_CODES::TASK: {
                break;
            }
            case RequestHandler::REQUEST_CODES::DELEGATE: {
                break;
            }
            case RequestHandler::REQUEST_CODES::FREE: {
                break;
            }
            case RequestHandler::REQUEST_CODES::DESOLATE: {
                break;
            }
            case RequestHandler::REQUEST_CODES::STATUS: {
                break;
            }
        }
    }
    return 1;
}