#ifndef UTILS_MISCELLANEOUS_H
#define UTILS_MISCELLANEOUS_H

#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>


std::vector<std::string> auto_tokenize(std::string line);

int init_subprocess(std::string params, std::string xname, std::string xpath);
bool is_global_request(std::string request);
bool is_global_command(std::vector<std::string> args);
bool is_request_belongs(std::vector<std::string> args, int worker_id);

#endif // !UTILS_MISCELLANEOUS_H