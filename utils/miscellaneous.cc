#include "miscellaneous.h"

int init_subprocess(const char *xpath, char *args[]) {
  int child_pid = fork();
  if (child_pid == 0) {
    char path[256];
    realpath(xpath, path);
    // for (int i = 0; args[i]; ++i) std::cout << args[i] << " ";
    std::cout << path << std::endl;
    if (execvp(path, args) == -1) {
      perror("Execv err");
    }
  }
  return child_pid;
}

std::string to_lower_str(std::string data) {
  for(auto& c : data) {
      c = tolower(c);
  }
  return data;
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

std::string append_filename_prefix(std::string path, std::string prefix) {
  std::string input_name   = path.substr(path.find_last_of('/') + 1);
  std::string input_dir    = path.substr(0, path.find_last_of('/'));
  return input_dir + "/" + prefix + input_name;
}