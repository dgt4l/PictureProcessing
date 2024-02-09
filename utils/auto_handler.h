#ifndef UTILS_AUTO_HANDLER_H
#define UTILS_AUTO_HANDLER_H

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

std::vector<std::string> auto_tokenize(std::string line);

template <typename T>
T auto_hash_item(const std::string item, const std::list<std::pair<T, std::string>> hasher) {
    std::string head = item.substr(0, item.find(" "));
    for (auto const& i : hasher) {
        if (!head.compare(i.second)) return i.first;
    }
    return hasher.front().first;
}

bool is_request_belongs(std::vector<std::string> args, int worker_id);

#endif // !UTILS_AUTO_HANDLER_H

