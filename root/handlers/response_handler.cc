#include "response_handler.h"

const std::string MESSAGE_PREFIX = "[ResponseHandler] ";

int ResponseHandler::read_response(const std::string response) {
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
                
                break;
            }

            case ResponseHandler::RESPONSE_CODES::STATUS: {
                break;
            }
        }
    }
    return 1;
}

std::string ResponseHandler::wait_response(int id, std::string type, std::string response) {
    if (response.length() > 0) {
        std::vector<std::string> args = auto_tokenize(response);
        std::cout << "[ResponseWaiter] " << "Recieved response: " << response << std::endl;
        if (args.at(0) == type &&
            args.at(1) == std::to_string(id)) {
                return response;
        }
    }
    return std::string();
}

void wait_response_thread(int id, std::string type, std::string* output) {
    std::string tmp_output;
    while (tmp_output.length() == 0) tmp_output = ResponseHandler::getInstance().wait_response(id, type, response);
    output = tmp_output;    
}