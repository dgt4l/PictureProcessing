#include "command_dispatcher.h"

const std::string MESSAGE_PREFIX = "[CommandDispatcher] ";

std::string getSecondWord(const std::string& str) {
    std::string secondWord = "";
    int wordCount = 0;
    std::string word;

    for (char c : str) {
        if (c != ' ') {
            word += c;
        } else {
            if (!word.empty()) {
                wordCount++;
                if (wordCount == 2) {
                    secondWord = word;
                    break;
                }
                word = "";
            } 
        }
    }

    if (!word.empty() && wordCount == 1) {
        secondWord = word;
        return secondWord;
    }
    return "empty";
}


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
  std::cout << MESSAGE_PREFIX << "Recieved command: " << cmd << std::endl;
  switch (auto_hash_item(cmd, hasher)) {
    case CommandDispatcher::CMD_CODES::EXEC: {
      if (count_workers() == 0) {
        std::cout << MESSAGE_PREFIX << "EXEC command has no effect: no worker available" << std::endl;
      }
      for (int i = 0; i < count_workers(); ++i) {
        pusher.send(zmq::buffer(cmd), zmq::send_flags::dontwait);
      }
      break;
    }
    case CommandDispatcher::CMD_CODES::EXIT: {
      
      raise(SIGINT);
    }
    case CommandDispatcher::CMD_CODES::CREATE: {
      if (!isInt(args.at(1))) {
        std::cout << MESSAGE_PREFIX << "DOLBAEB CHISLO VVODI NAHUI" << std::endl;
        break;
      }
      if (is_reserved(stoi(args.at(1)))) {
        std::cout << MESSAGE_PREFIX << "worker with id " << args.at(1) << " already exists" << std::endl;
        break;
      }
      char *worker_args[] = {"client", args.at(1).data(), NULL};
      init_subprocess("client/client", worker_args);
      break;
    }
    case CommandDispatcher::CMD_CODES::UNKNOWN: {
      std::cout << MESSAGE_PREFIX << "ahahhahahahahahha vot eblan" << std::endl;
      break;
    }
    default: {
      std::cout << "xd" << std::endl;
      break;
    }
  }
  return 1;
}

int CommandDispatcher::dispatch_command() {
  std::string cmd;
  std::getline(std::cin, cmd);
  return dispatch_command(cmd);
}
