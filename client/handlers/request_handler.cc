#include "request_handler.h"

const std::string MESSAGE_PREFIX = "[RequestHandler] ";

int RequestHandler::read_request() {
    std::string request = s_recv(puller, ZMQ_DONTWAIT);
    if (request.length() > 0) {
        std::vector<std::string> args = auto_tokenize(request);
        std::string request_type = args.at(1);
        if (is_request_belongs(args, worker.id)) {
            std::cout << MESSAGE_PREFIX << "Recieved request: " << request_type << std::endl;
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
                        Когда воркер достигает Status::COMPLETE, он отправляет воркеру Response::COMPLETE
                    */
                    if (worker.s == Worker::Status::IDLE) {
                        worker.set_task(args.at(3), auto_hash_item(args.at(4), worker.task.filter_hasher));
                        worker.s = Worker::Status::READY;
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
                        ! НЕ РЕАЛИЗОВАНО:    
                    //    ! Но если количество ресурсов, которое было выставлено сервером(в любой момент), 
                    //    ! окажется недостаточным для работы, воркер вернет Status::SHORTAGE
                    */
                    int request_amount = stoi(args.at(3));
                    if (request_amount >= 0) {
                        switch (worker.s) {
                            case Worker::Status::READY: {
                                worker.limit = request_amount;
                                // ! working process
                                worker.s = Worker::Status::WORKING;
                                break;
                            }
                            case Worker::Status::WORKING: {
                                worker.limit = request_amount;
                                // ! pause inner request
                                break;
                            }
                            case Worker::Status::PAUSED: {
                                worker.limit = request_amount;
                                // ! depause inner request
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
                    int request_amount = stoi(args.at(3));
                    
                    switch(worker.s){
                        case Worker::Status::IDLE: {
                            SimpleResponse r(Response::TRANSFER, std::to_string(worker.id));
                            r.dispatch_response();
                            break;
                        }
                        case Worker::Status::READY: {
                            SimpleResponse r(Response::TRANSFER, std::to_string(worker.id));
                            r.dispatch_response();
                            break;
                        }
                        case Worker::Status::WORKING: {
                            // ! pause inner request
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
