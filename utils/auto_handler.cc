#include "auto_handler.h"


std::vector<std::string> auto_tokenize(std::string line) {
    std::stringstream ssl(line);
    std::string token;
    std::vector<std::string> args;
    while (getline(ssl, token, ' ')) {
        args.push_back(token);
    }
    return args;
}

bool is_request_belongs(std::vector<std::string> args, int worker_id) {
    return args.size() <= 2 || stoi(args.at(2)) == worker_id;
}