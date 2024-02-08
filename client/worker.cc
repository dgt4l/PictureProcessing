// #include "../zmq/command_dispatcher.h"
#include "worker.h"

zmq::context_t ctx;
zmq::socket_t pusher(ctx, zmq::socket_type::push);
zmq::socket_t puller(ctx, zmq::socket_type::pull);

const std::string SERVER_PUSHER_SOCKET_PATTERN = "tcp://127.0.0.1:5555";
const std::string SERVER_PULLER_SOCKET_PATTERN = "tcp://127.0.0.1:5556";

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("got %d args, exiting\n", argc);
    return 1;
  }
  std::string id = argv[1];
  std::string pid = std::to_string(getpid());
  zmq::message_t msg;
  // Worker w(Worker::Status::IDLE, id);
  puller.connect(SERVER_PUSHER_SOCKET_PATTERN);
  pusher.connect(SERVER_PULLER_SOCKET_PATTERN);
  std::string cmd = "IAMALIVE " + id + " " + pid;
  pusher.send(zmq::buffer(cmd), zmq::send_flags::dontwait);

  while (1) {
    puller.recv(&msg);
    std::string rpl = std::string(static_cast<char*>(msg.data()), msg.size());

    std::cout << "[" << id << "] " << rpl << std::endl;
  }
  return 0;
}
//   int id = atoi(argv[1]);
//   imalive(id);
//   void *context = zmq_ctx_new();
//   // void *puller = init_socket(context, id, ZMQ_PULL);
//   // void *root_pusher = init_socket(context, ROOT_ID, ZMQ_PUSH);
//   void *sibling_pusher = NULL;
//   void *son_pusher = NULL;

//   char buffer[1000];
//   char subcommand[1000], cmd[1000];
//   int size = 0, param_id, param_parent_id, param_sibling_id, param_son_id,
//       param_delay;

//   while (1) {
//     size = zmq_recv(puller, buffer, sizeof(buffer) + 2, ZMQ_DONTWAIT);
//     if (size != -1) {
//       printf("[%d] Recieved command: %s\n", id, buffer);
//     }
//   }
//   return 0;
// }

