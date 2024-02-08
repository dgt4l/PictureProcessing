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

template <typename T>
T auto_hash_item(const std::string item, const std::list<std::pair<T, std::string>> hasher) {
    std::string head = item.substr(0, item.find(" "));
    std::cout << "[" << head << "]" << std::endl;
    for (auto const& i : hasher) {
        if (!head.compare(i->second)) return i->first;
    }
    return hasher.front()->first;
}
