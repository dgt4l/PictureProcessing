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

int CommandDispatcher::dispatch_command() {
  std::string cmd;
  std::getline(std::cin, cmd);
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
      // ! response handler not ending
      return 0;
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
      init_worker_subprocess(args.at(1));
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

void init_worker_subprocess(std::string id) {
  int child_pid = fork();
  if (child_pid == 0) {
    char path[256];
    char *args[] = {"client", id.data(), NULL};
    realpath("client/client", path);
    printf(path);
    if (execvp(path, args) == -1) {
      perror("Execv err");
    }
  }
}

// char *solve_address(int id) {
//   char *buf = (char *)malloc(sizeof(SOCKET_PATTERN) + 10);
//   sprintf(buf, "%s%d", SOCKET_PATTERN, id + SRC_PORT);
//   return buf;
// }

// void *init_socket(void *context, int id, int type) {
//   void *socket = zmq_socket(context, type);
//   char *address = solve_address(id);
//   switch (type) {
//     case ZMQ_PULL:
//       if (zmq_bind(socket, address)) perror("zmq_bind");
//       break;
//     case ZMQ_PUSH:
//       if (zmq_connect(socket, address)) perror("zmq_connect");
//       break;
//   }
//   free(address);
//   return socket;
// }

// int send_message(void *pusher, char *buffer, size_t size) {
//   if (zmq_send(pusher, buffer, size, ZMQ_DONTWAIT) == -1) {
//     perror("zmq_send");
//     return 0;
//   }
//   return 1;
// }


// int solve_server_cmd(std::string const& inString) {
//   if (!inString.compare("")) return EXIT;
//   if (!inString.compare("create")) return CREATE;
//   if (!inString.compare("terminate")) return TERMINATE;
//   if (!inString.compare("task")) return TASK;
//   if (!inString.compare("desolate")) return DESOLATE;
//   if (!inString.compare("delegate")) return DELEGATE;
//   if (!inString.compare("available")) return AVAILABLE;
//   if (!inString.compare("status")) return STATUS;
//   if (!inString.compare("free")) return FREE;
//   return UNKNOWN;
// }
