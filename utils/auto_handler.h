#ifndef UTILS_AUTO_HANDLER_H
#define UTILS_AUTO_HANDLER_H

#include <list>
#include <string>
#include <vector>

template <typename T>
T auto_hash_item(const std::string item, const std::list<std::pair<T, std::string>> hasher) {
    std::string head = item.substr(0, item.find(" "));
    for (auto const& i : hasher) {
        if (!head.compare(i.second)) return i.first;
    }
    return hasher.front().first;
}

template <typename T>
std::string auto_dehash_item(T item, const std::list<std::pair<T, std::string>> hasher) {
    for (auto const& i : hasher) {
        if (item == i.first) return i.second;
    }
    return hasher.front().second;
}

#endif // !UTILS_AUTO_HANDLER_H

