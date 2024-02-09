#include "request_handler.h"

const std::string MESSAGE_PREFIX = "[RequestHandler] ";

int RequestHandler::read_request() {
    std::string request = s_recv(puller, ZMQ_DONTWAIT);
    if (request.length() > 0) {
        std::vector<std::string> args = auto_tokenize(request);
        std::cout << MESSAGE_PREFIX << "Recieved request: " << args.at(0) << std::endl;
        switch (auto_hash_item(request, hasher)) {
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