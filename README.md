# PictureProcessing

```
class Requester {
    private:
        socket_t s;
    public:
        Requester::Requester();
        Requester::send_request();
        Requester::listen_response();
        enum class REQUEST_TYPE {
        ...
        };
};
```
```
class Responser {
    private:
        socket_t s;
    public:
        Responser::Responser();
        Responser::listen_request();
        Responser::send_response();
        enum class RESPONSE_TYPE {
        ...
        };
};
```

ЗАПРОСЫ

Requester::CREATE
Создает подпроцесс воркера с установленным [id]. 
При успешном создании, новый воркер получает статус Status::IDLE, 
инициализирует свой пуллер и возвращает серверу Responser::IAMALIVE.

Requester::TERMINATE
Заставляет воркера [id] досрочно прервать свою работу. 
Если воркер не отвечает, убивает самосоятельно по его pid.
Может быть вызвано без id - тогда все воркеры будут уничтожены тем же способом
При успешно удалении возвращает Responser::DEAD

Requester::TASK
Воркер [id] получает Task, выраженный его параметрами: [input_path] [filter].
Если воркер в Status:IDLE, он сохраняет себе эту задачу и переводится в Status::READY.
Если воркер в других статусах, он остается в том же статусе, и возвращает Responser::BUSY.
Когда воркер достигает Status::COMPLETE, он отправляет воркеру Response::COMPLETE

Requester::DELEGATE
Воркеру [id] представляются [amount] ресурсов для его распоряжения.
Если [amount] < 0, воркеры не меняют свой лимит.
Воркер в Status::IDLE или Status::COMPLETE не принимает ресурсы и возвращает Responser::TOOSWEET
Воркер в Status::READY принимает ресурсы и устанавливает внутренний лимит ресурсов на это значение, начинает работу и пeреходит в Status::Working.
Воркер в Status::WORKING меняет лимит на новый, статус не меняет, перед новой итерации перейдет в Status::PAUSED.
Воркер в Status::PAUSED пытается начать работать с новым статусом
Но если количество ресурсов, которое было выставлено сервером(в любой момент), окажется недостаточным для работы, воркер вернет Status::SHORTAGE

Requester::STATUS
Воркер [id] возвращает Response::STATUS
Может быть вызвано без [id] - тогда все воркеры вернут свой Response::STATUS

Requester::FREE
Воркер [id] завершает свою работу если находится в определенном статусе.
Воркер в Status::COMPLETE, воркер завершает свою работу и возвращает Responser::DEAD.
Воркер в другом статусе возвращает Responser::BUSY.

Requester::DESOLATE
Воркер [id] изменяет свой лимит ресурсов <n> таким образом, чтобы хватило на выполнение текущей задачи.
Воркер в Status::IDLE или Status::READY не имеет свободных ресурсов, возвращает Status::TRANSFER(0)
Воркер в Status::WORKING сохраняет этот запрос и при окончании текущей итерации получит Status::PAUSED.
Воркер в Status::PAUSED отправляет запрос Responser::TRANSFER и ожидает DELEGATE запроса для продолжения.

Responser::IAMALIVE
Возвращается при начале жизни воркера. Содержит в себе [id] и [pid] созданного воркера.

Responser::DEAD
Возвращается когда воркер подтверждает, что процесс будет завершен. Содержит в себе свой [id].

Responser::COMPLETE
Возвращается, когда воркер успешно завершает свою задачу. Содержит в себе свой [id] и [output_path].

Responser::BUSY
Возвращается, когда статус воркера мешает ему принять задачу. Содержит в себе свой [id].

Responser::TOOSWEET
Возвращается, когда воркеру без дела предлагают ресурсы. Для чего? Содержит в себе свой [id].

Responser::SHORTAGE
Возвращается, когда ресурсов для начала работы недостаточно. Содержит в себе свой [id].

Responser::TRANSFER
Возвращается, когда воркер освободил ресурсы и может их передать серверу. Содержит в себе [id] и [amount] >= 0

Responser::STATUS
Возвращается, когда воркер предоставляет свои данные. Содержит в себе [id], [pid], [limit], [status] и т.п.
