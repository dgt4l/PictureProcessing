#include "command_dispatcher.h"

const std::string MESSAGE_PREFIX = "\e[0;32m[CommandDispatcher]\e[0m\t\t ";

void dispatch_command_thread(std::string cmd, std::vector<std::string> args, const std::list<std::pair<enum CommandDispatcher::CMD_CODES, std::string>> hasher);

bool isInt(const std::string& str) {
  for (char ch : str) {
    if (!isdigit(ch)) {
      return false;
    }
  }
  return true;
}

int CommandDispatcher::dispatch_command(std::string cmd) {
  std::vector<std::string> args = auto_tokenize(cmd);
  std::cout << MESSAGE_PREFIX << "Recieved command: \e[0;95m" << cmd << "\e[0m" << std::endl;
  std::thread dct(dispatch_command_thread, cmd, args, hasher);
  dct.detach();
  return 1;
}

void dispatch_command_thread(std::string cmd, std::vector<std::string> args, const std::list<std::pair<enum CommandDispatcher::CMD_CODES, std::string>> hasher) {
  switch (auto_hash_item(cmd, hasher)) {
    case CommandDispatcher::CMD_CODES::EXEC: {
      if (worker_map.count_workers() == 0) {
        std::cout << MESSAGE_PREFIX
                  << "EXEC command has no effect: no worker available"
                  << std::endl;
      }
      std::cout << MESSAGE_PREFIX << "Sending command " << worker_map.count_workers() << " times" << std::endl;
      for (int i = 0; i < worker_map.count_workers(); ++i) {
        pusher.send(zmq::buffer(cmd), zmq::send_flags::dontwait);
      }
      break;
    }
    case CommandDispatcher::CMD_CODES::EXIT: {
      // for (int i = 0; i) raise(SIGINT);
      break;
    }
    case CommandDispatcher::CMD_CODES::CREATE: {
      if (!isInt(args.at(1))) {
        std::cout << MESSAGE_PREFIX << "invalid type of workers number"
                  << std::endl;
        break;
      }
      if (worker_map.is_reserved(stoi(args.at(1)))) {
        std::cout << MESSAGE_PREFIX << "worker with id " << args.at(1)
                  << " already exists" << std::endl;
        break;
      }
      char* worker_args[] = {"client", args.at(1).data(), NULL};
      init_subprocess("client/client", worker_args);
      break;
    }
    case CommandDispatcher::CMD_CODES::UNKNOWN: {
      std::cout << MESSAGE_PREFIX << "unknown command" << std::endl;
      break;
    }
  }
}

int CommandDispatcher::dispatch_command() {
  std::string cmd;
  std::getline(std::cin, cmd);
  return dispatch_command(cmd);
}
