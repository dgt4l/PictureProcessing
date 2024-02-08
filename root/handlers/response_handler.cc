#include "response_handler.h"

const std::string MESSAGE_PREFIX = "[ResponseHandler] ";

enum ResponseHandler::RESPONSE_CODES ResponseHandler::solve_response(const std::string response) {
  std::string head = response.substr(0, response.find(" "));
  std::cout << "[" << head << "]" << std::endl;
  if (!head.compare("IAMALIVE")) return ResponseHandler::RESPONSE_CODES::IAMALIVE;
  if (!head.compare("DEAD")) return ResponseHandler::RESPONSE_CODES::DEAD;
  if (!head.compare("COMPLETE")) return ResponseHandler::RESPONSE_CODES::BUSY;
  if (!head.compare("TOOSWEET")) return ResponseHandler::RESPONSE_CODES::TOOSWEET;
  if (!head.compare("SHORTAGE")) return ResponseHandler::RESPONSE_CODES::SHORTAGE;
  if (!head.compare("TRANSFER")) return ResponseHandler::RESPONSE_CODES::TRANSFER;
  if (!head.compare("STATUS")) return ResponseHandler::RESPONSE_CODES::STATUS;
  if (!head.compare("COMPLETE")) return ResponseHandler::RESPONSE_CODES::COMPLETE;

  return ResponseHandler::RESPONSE_CODES::UNKNOWN;
}

int ResponseHandler::read_response() {
    std::string response = s_recv(puller, ZMQ_DONTWAIT);
    if (response.length() > 0) {
        std::vector<std::string> args = auto_tokenize(response);
        std::cout << MESSAGE_PREFIX << "Recieved response: " << args.at(0) << std::endl;
        switch (solve_response(response)) {
            case ResponseHandler::RESPONSE_CODES::UNKNOWN: {
                break;
            }
            case ResponseHandler::RESPONSE_CODES::IAMALIVE: {
                append_worker(stoi(args.at(1)), stoi(args.at(2)));
                break;
            }
            case ResponseHandler::RESPONSE_CODES::DEAD:{
                
                break;
            }
            case ResponseHandler::RESPONSE_CODES::COMPLETE:{
                
                break;
            }
            case ResponseHandler::RESPONSE_CODES::BUSY:{
                
                break;
            }
            
            case ResponseHandler::RESPONSE_CODES::TOOSWEET:{
                
                break;
            }
            case ResponseHandler::RESPONSE_CODES::SHORTAGE:{
                
                break;
            }
            case ResponseHandler::RESPONSE_CODES::TRANSFER:{
                
                break;
            }
        }
    }
    return 1;
}
