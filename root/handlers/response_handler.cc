#include "response_handler.h"

const std::string MESSAGE_PREFIX = "[ResponseHandler] ";

int ResponseHandler::read_response() {
    std::string response = s_recv(puller, ZMQ_DONTWAIT);
    if (response.length() > 0) {
        std::vector<std::string> args = auto_tokenize(response);
        std::cout << MESSAGE_PREFIX << "Recieved response: " << response << std::endl;
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
                CommandDispatcher::getInstance().dispatch_command("EXEC FREE " + args.at(1));
                break;
            }
            case ResponseHandler::RESPONSE_CODES::BUSY: {
                break;
            }
    
            case ResponseHandler::RESPONSE_CODES::TOOSWEET: {
                break;
            }
            case ResponseHandler::RESPONSE_CODES::SHORTAGE: {
                break;
            }
            case ResponseHandler::RESPONSE_CODES::TRANSFER: {
                break;
            }
            case ResponseHandler::RESPONSE_CODES::STATUS: {
                break;
            }
        }
    }
    return 1;
}
