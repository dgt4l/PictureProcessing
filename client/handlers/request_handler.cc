#include "request_handler.h"

int RequestHandler::read_request(std::string request) {
    std::string MESSAGE_PREFIX = "\e[0;36m[RequestHandler:" + std::to_string(worker.id) + "]\e[0m\t\t ";
    // std::string request = s_recv(puller, ZMQ_DONTWAIT);
    if (request.length() > 0) {
        std::cout << MESSAGE_PREFIX << "Recieved request: \e[0;95m" << request << "\e[0m" << std::endl;
        std::vector<std::string> args = auto_tokenize(request);
        std::string request_type = args.at(1);
        if (is_request_belongs(args, worker.id)) {
            switch (auto_hash_item(request_type, hasher)) {
                case RequestHandler::REQUEST_CODES::UNKNOWN: {
                    std::cout << MESSAGE_PREFIX << " Unknown request, ignoring it" << std::endl;
                    break;
                }
                case RequestHandler::REQUEST_CODES::TERMINATE: {
                    /*
                        ? TERMINATE [id]
                        Заставляет воркера [id] досрочно прервать свою работу. 
                        Если воркер не отвечает, убивает самосоятельно по его pid.
                        Может быть вызвано без id - тогда все воркеры будут уничтожены тем же способом
                        При успешно удалении возвращает Responser::DEAD
                    */
                    return 0;
                }
                case RequestHandler::REQUEST_CODES::TASK: {
                    /*
                        ? EXEC TASK <id> <input_path> <filter>
                        Воркер [id] получает Task, выраженный его параметрами: [input_path] [filter].
                        Если воркер в Status:IDLE, он сохраняет себе эту задачу и переводится в Status::READY.
                        Если воркер в других статусах, он остается в том же статусе, и возвращает Responser::BUSY.
                    */

                    if(args.size() != 5){
                        std::cout << MESSAGE_PREFIX << "Error: EXEC TASK <id> <input_path> <filter> \n" << std::endl;
                        break;
                    }

                    if (worker.s == Worker::Status::IDLE) {
                        worker.set_task(args.at(3), auto_hash_item(args.at(4), worker.task.filter_hasher));
                        worker.s = Worker::Status::READY;
                        SimpleResponse r(Response::GOTWORK, std::to_string(worker.id));
                        r.dispatch_response();
                    } else {
                        SimpleResponse r(Response::BUSY, std::to_string(worker.id));
                        r.dispatch_response();
                    }
                    break;
                }
                case RequestHandler::REQUEST_CODES::DELEGATE: {
                    /*
                        ? EXEC DELEGATE <id> <amount>
                        Воркеру [id] представляются [amount] ресурсов для его распоряжения.
                        Если [amount] < 0, воркеры не меняют свой лимит.
                        Воркер в Status::IDLE или Status::COMPLETE не принимает ресурсы и возвращает Responser::TOOSWEET
                        Воркер в Status::READY принимает ресурсы и устанавливает внутренний лимит ресурсов на это значение,
                            начинает работу и пeреходит в Status::Working.
                        Воркер в Status::WORKING меняет лимит на новый, статус не меняет, перед новой итерации перейдет в Status::PAUSED.
                        Воркер в Status::PAUSED пытается начать работать с новым статусом
                        Когда воркер достигает Status::COMPLETE, он отправляет воркеру Response::COMPLETE
                        ! НЕ РЕАЛИЗОВАНО:    
                    //    ! Но если количество ресурсов, которое было выставлено сервером(в любой момент), 
                    //    ! окажется недостаточным для работы, воркер вернет Status::SHORTAGE
                    */

                    if(args.size() != 4){
                        std::cout << MESSAGE_PREFIX << "Error: EXEC DELEGATE <id> <amount> \n" << std::endl;
                        break;
                    }

                    int request_amount = stoi(args.at(3));
                    if (request_amount >= 0) {
                        switch (worker.s) {
                            case Worker::Status::READY: {
                                worker.limit = request_amount;
                                worker.s = Worker::Status::WORKING;
                                std::thread wt_thread(working_thread);
                                wt_thread.detach();
                                break;
                            }
                            case Worker::Status::WORKING: {
                                worker.limit = request_amount;
                                worker.s = Worker::Status::PAUSED;
                                break;
                            }
                            case Worker::Status::PAUSED: {
                                worker.limit = request_amount;
                                worker.s = Worker::Status::WORKING;
                                break;
                            }
                            case Worker::Status::COMPLETE:
                            case Worker::Status::IDLE: {
                                std::vector<std::string> params = {std::to_string(worker.id)};
                                SimpleResponse r(Response::TOOSWEET, std::to_string(worker.id));
                                r.dispatch_response();
                                break;
                            }
                        }
                    }
                    break;
                }
                case RequestHandler::REQUEST_CODES::FREE: {
                    /*
                        ? EXEC FREE <id>
                        Воркер [id] завершает свою работу если находится в определенном статусе.
                        Воркер в Status::COMPLETE, воркер завершает свою работу и возвращает Responser::DEAD.
                        Воркер в другом статусе возвращает Responser::BUSY.
                    */
                    if (args.size() != 3){
                        std::cout << MESSAGE_PREFIX << "FREE Request must have an id" << std::endl;
                        break;
                    }
                    if (worker.s == Worker::Status::COMPLETE){
                        return 0;
                    } else {
                        SimpleResponse r(Response::BUSY, std::to_string(worker.id));
                        r.dispatch_response();
                        break;
                    }
                    break;
                }
                case RequestHandler::REQUEST_CODES::DESOLATE: {
                    /*
                        ? EXEC DESOLATE <id> <amount>
                        Воркер [id] изменяет свой лимит ресурсов на <amount> таким образом, чтобы хватило на выполнение текущей задачи.
                        Воркер в Status::IDLE или Status::READY не имеет свободных ресурсов, возвращает Status::TRANSFER(0)
                        Воркер в Status::WORKING сохраняет этот запрос и при окончании текущей итерации получит Status::PAUSED.
                        Воркер в Status::PAUSED отправляет запрос Responser::TRANSFER и ожидает DELEGATE запроса для продолжения.
                    */

                    // int request_amount = stoi(args.at(3));
                    
                    switch(worker.s) {
                        case Worker::Status::IDLE:
                        case Worker::Status::READY: {
                            // int free_amount = 0;
                            // std::vector <std::string> params = {
                            //     {std::to_string(worker.id)},
                            //     {std::to_string(free_amount)}
                            // };
                            // ComplexResponse r(Response::TRANSFER, params);
                            // r.dispatch_response();
                            break;
                        }
                        case Worker::Status::WORKING: {
                            worker.s = Worker::Status::PAUSED;
                            int free_amount = worker.limit;
                            std::vector <std::string> params = {
                                {std::to_string(worker.id)},
                                {std::to_string(free_amount)}
                            };
                            ComplexResponse r(Response::TRANSFER, params);
                            r.dispatch_response();
                            break;
                        }
                        case Worker::Status::PAUSED: {
                            break;
                        }
                    }
                    break;
                }
                case RequestHandler::REQUEST_CODES::STATUS: {
                    /*
                        ? EXEC STATUS // EXEC STATUS [id]
                        Воркер [id] возвращает Response::STATUS
                        Может быть вызвано без [id] - тогда все воркеры вернут свой Response::STATUS
                    */
                    
                    std::vector<std::string> params = {
                        {std::to_string(worker.id)},
                        {std::to_string(getpid())},
                        {worker.task.input_path},
                        {auto_dehash_item(worker.task.filter, worker.task.filter_hasher)},
                        {auto_dehash_item(worker.s, worker.status_hasher)}
                    };
                    ComplexResponse r(Response::STATUS, params);
                    r.dispatch_response();
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return 1;
}

void working_thread() {
    for (int i = 0; i < 3; ++i) {
        pid_t pid = working_iteration(i);
        int status;
        while (-1 == waitpid(pid, &status, 0)) {
            // usleep(10);
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cout << "[WorkingThread] Error completion iteration: " << i << ", process pid failed: " << pid << std::endl;
            break;
        }
        while (worker.s == Worker::Status::PAUSED) {
            // usleep(10);
        }
    }
    std::vector<std::string> params = {
        {std::to_string(worker.id)}, 
        {append_filename_prefix(worker.task.input_path, "output_")}
    };
    worker.s = Worker::Status::COMPLETE;
    ComplexResponse r(Response::COMPLETE, params);
    r.dispatch_response();
}

int working_iteration(int channel) {
    std::string upper_filter = auto_dehash_item(worker.task.filter, worker.task.filter_hasher);
    std::string input_path = worker.task.input_path;
    std::string output_path  = append_filename_prefix(input_path, "output_");
    if (channel != 0) input_path = output_path;
    char *mtci_args[] = {
        "mtci_main",
        "-i", input_path.data(),
        "-o", output_path.data(),
        "-t", to_lower_str(upper_filter).data(),
        "-j", std::to_string(worker.limit).data(),
        "-c", std::to_string(channel).data(),
        NULL
    };
    return init_subprocess("mtci/mtci_main", mtci_args);
}
