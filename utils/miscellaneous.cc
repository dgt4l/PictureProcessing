#include "miscellaneous.h"

int init_subprocess(std::string params, std::string xname, std::string xpath) {
  int child_pid = fork();
  if (child_pid == 0) {
    char path[256];
    char *args[] = {xname.data(), params.data(), NULL};
    realpath(xpath.data(), path);
    if (execvp(path, args) == -1) {
      perror("Execv err");
    }
  }
  return child_pid;
}

std::vector<std::string> auto_tokenize(std::string line) {
    std::stringstream ssl(line);
    std::string token;
    std::vector<std::string> args;
    while (getline(ssl, token, ' ')) {
        args.push_back(token);
    }
    return args;
}

bool is_global_request(std::string request) {
    return (!request.compare("TERMINATE")) ||
           (!request.compare("STATUS"))    ||
           (!request.compare("DESOLATE"));
}

bool is_global_command(std::vector<std::string> args) {
    return args.size() <= 2 && is_global_request(args.at(1));
}

bool is_request_belongs(std::vector<std::string> args, int worker_id) {
    return is_global_command(args) || stoi(args.at(2)) == worker_id;
}