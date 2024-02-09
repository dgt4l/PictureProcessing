#include "request_handler.h"

const std::string MESSAGE_PREFIX = "[RequestHandler] ";

int RequestHandler::read_request() {
    std::string request = s_recv(puller, ZMQ_DONTWAIT);
    if (request.length() > 0) {
        std::vector<std::string> args = auto_tokenize(request);
        std::string request_type = args.at(1);
        if (is_request_belongs(args, worker.id)) {
            std::cout << MESSAGE_PREFIX << "Recieved request: " << request_type << std::endl;
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
                    ResponseDispatcher::getInstance().resps << "STATUS " << worker.id << " " << worker.limit;
                    std::cout << "xd";
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return 1;
}
