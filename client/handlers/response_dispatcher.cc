#include "response_dispatcher.h"

const std::string MESSAGE_PREFIX = "[ResponseDispatcher] ";

int ResponseDispatcher::dispatch_response() {
  std::string cmd;
  std::getline(resps, cmd);
  if (cmd.length() > 0) {
    std::vector<std::string> args = auto_tokenize(cmd);
    std::cout << MESSAGE_PREFIX << "Worker dispatching response: " << cmd << std::endl;
    pusher.send(zmq::buffer(cmd), zmq::send_flags::dontwait);
    resps.str("");
  }
}
