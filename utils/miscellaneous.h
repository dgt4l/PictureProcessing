#ifndef UTILS_MISCELLANEOUS_H
#define UTILS_MISCELLANEOUS_H

#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<std::string> auto_tokenize(std::string line);

int init_subprocess(const char *xpath, char *args[]);
bool is_global_request(std::string request);
bool is_global_command(std::vector<std::string> args);
bool is_request_belongs(std::vector<std::string> args, int worker_id);
std::string to_lower_str(std::string data);
std::string append_filename_prefix(std::string path, std::string prefix);

#endif // !UTILS_MISCELLANEOUS_H