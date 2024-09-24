// #include "../zmq/command_dispatcher.h"
#include "worker.h"

zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::pull);

Worker worker;

const std::string SERVER_PUSHER_SOCKET_PATTERN = "tcp://127.0.0.1:5555";
const std::string SERVER_PULLER_SOCKET_PATTERN = "tcp://127.0.0.1:5556";

void request_handler_thread() {
  std::string request = s_recv(puller, ZMQ_DONTWAIT);
  while (RequestHandler::getInstance().read_request(request)) {
    request = s_recv(puller, ZMQ_DONTWAIT);
  }
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

  std::vector <std::string> alive_params = {id, pid};
  ComplexResponse alive_r(Response::IAMALIVE, alive_params);
  alive_r.dispatch_response();

  std::thread rh_thread(request_handler_thread);

  rh_thread.join();

  puller.close();
  
  SimpleResponse dead_r(Response::DEAD, id);
  dead_r.dispatch_response();

  pusher.close();
  return 0;
}
