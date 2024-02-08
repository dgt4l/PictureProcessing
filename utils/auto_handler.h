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
T auto_hash_item(const std::string item, const std::list<std::pair<T, std::string>> hasher);


#endif // !UTILS_AUTO_HANDLER_H