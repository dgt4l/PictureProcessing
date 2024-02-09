// #include "../zmq/command_dispatcher.h"
#include "worker.h"

zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::pull);

Worker worker;

const std::string SERVER_PUSHER_SOCKET_PATTERN = "tcp://127.0.0.1:5555";
const std::string SERVER_PULLER_SOCKET_PATTERN = "tcp://127.0.0.1:5556";

void response_dispatcher_thread() {
  while (ResponseDispatcher::getInstance().dispatch_response())
    ;
}

void request_handler_thread() {
  while (RequestHandler::getInstance().read_request())
    ;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("got %d args, exiting\n", argc);
    return 1;
  }
  std::string id = argv[1];
  std::string pid = std::to_string(getpid());

  worker = Worker(Worker::Status::IDLE, stoi(id));
  
  puller.connect(SERVER_PUSHER_SOCKET_PATTERN);
  pusher.connect(SERVER_PULLER_SOCKET_PATTERN);

  ResponseDispatcher::getInstance().resps << "IAMALIVE " + id + " " + pid;

  std::thread rd_thread(response_dispatcher_thread);
  std::thread rh_thread(request_handler_thread);

  rd_thread.join();
  rh_thread.join();
  
  // pusher.send(zmq::buffer(cmd), zmq::send_flags::dontwait);
  return 0;
}
