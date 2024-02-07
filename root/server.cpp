#include "zmq/helpers.h"

const int MAX_CMD_LENGTH = 100;
const int BUFFER_SIZE = 2048;
const char SERVER_PREFIX[] = "[Server]";

int main() {
  void *context = zmq_ctx_new();
  // * Puller - passively listening socket
  // * It connects to the static addres, which is defined by node's id
  // * This setups unique address for each node, and every node could connect to
  // * another via id only
  void *puller = init_socket(context, ROOT_ID, ZMQ_PULL);
  // * Pusher - actively requesting socket
  // * In order to send message to other node, its connects to this node's
  // * address, holded by other node's puller, and transfer message to it
  std::vector <void *> client_pushers;
  std::string cmd;
  printf("%s Server launched\n", SERVER_PREFIX);
  int server_status = ONLINE, param_id;
  while (server_status) {
    std::getline(std::cin, cmd);
    switch (solve_server_cmd(cmd)) {
      case EXIT: {
        // * we need to terminate workers too
        server_status = CLOSED;
        break;
      }
      case UNKNOWN: {
        printf("%s ahahaha ebik naberi norm command\n", SERVER_PREFIX);
        break;
      }
      case CREATE: {
        if (sscanf(buffer.data(), "%s %d", cmd, &param_id) == 2) {
          char *args[] = {"child", std::to_string(param_id).data()};
          printf("%s", std::to_string(param_id).data());
          init_worker_subprocess(args);
        }
        break;
      }
      case STATUS: {
        // * gets worker status - is it holding a task, how many resources,
        // * is it available, etc
        break;
      }
      case TERMINATE: {
        // * destroys worker by id ignoring its tasks, freeing its resources
        // * if worker hold a task, server still will be warned
        break;
      }
      case DESOLATE: { // ? <----------- spectre passive???????????????????????
        // * tries to remove worker, freeing its resources
        // * if worker hold a task, worker will return all of it resources, 
        // * and held only neccessary ones
        break;
      }
      case FREE: { // ? <--------------- bird yeahhhhh tu tu du duddu ddududududududd duduuDUDUDUSUYSUUDSUSDUSDUSDUsduiopuapoiudoakj
        // * if worker completed task, resources will 
      }
      case TASK: {
        // * delegate specified worker a task, returns how much resources it needs
        // * if it specified, worker will be terminated after its completion
        // * if worker got a task and no resources, it will patiently wait with task freezed
        // * after task completion
        break;
      }
      case DELEGATE: {
        // * worker recieves exactly this much resources
        // * if worker had a task, it will begin completing it
        // * if worker completing task
        // * else resources ignored
        break;
      }
      case AVAILABLE: {
        // * returns sum of resources in worker net
        break;
      }
    }
    printf("%s Command: %s", SERVER_PREFIX, buffer);
    zmq_sleep(2);
  }
  zmq_close(puller);
  zmq_ctx_destroy(context);
  return 0;
}
