#ifndef SSERVER_H
#define SSERVER_H
#include "includes.h"



class Road;

class State {
    /**
     * @var Road
     */
protected:
    Road* Road_;

public:
    virtual ~State() {
    }

    void set_Road(Road* Road) {
        this->Road_ = Road;
    }

    virtual void Passed() = 0;
    virtual void Timeout() = 0;
    
    //virtual void ManualPass() = 0;
};

/**
 * Класс дороги определяет интерфейс, представляющий интерес для клиентов. Он также
 * хранит ссылку на экземпляр подкласса Состояния, который отображает текущее
 * состояние Класса дороги.
 */
class Road {
    /**
     * @var State Ссылка на текущее состояние Класса дороги.
     */
private:
    State* state_;

public:
    Road(State* state) : state_(nullptr) {
        this->TransitionTo(state);
    }
    ~Road() {
        delete state_;
    }
    /**
     * Класс дороги позволяет изменять объект Состояния во время выполнения.
     */
    void TransitionTo(State* state) {
        std::cout << "Status change to : " << typeid(*state).name() << ".\n";
        if (this->state_ != nullptr)
            delete this->state_;
        this->state_ = state;
        this->state_->set_Road(this);
    }
    /**
     * Класс дороги делегирует часть своего поведения текущему объекту Состояния.
     */
    void Request1() {
        this->state_->Passed();
    }
    void Request2() {
        this->state_->Timeout();
    }
};

/**
 * Конкретные Состояния реализуют различные модели поведения, связанные с
 * состоянием Класса дороги.
 */
class IdleState : public State {
public:
    void Passed() override;

    void Timeout() override {
    }
};

class CheckPassCardState : public State {
public:
    void Passed() override {
    }
    void Timeout() override {
        this->Road_->TransitionTo(new IdleState);
    }
};


//Буффер для приёма данных от клиента

static constexpr uint16_t buffer_size = 4096;

struct CommandData
{
    int Test1;
    int Test2;
    char start_time;
};

struct CommandOut
{
    int Status;
};


struct TcpServer {
    class Client;
    //Тип Callback-функции обработчика клиента
    typedef std::function<void(Client)> handler_function_t;
    //Статус сервера
    enum class status : uint8_t {
        up = 0,
        err_socket_init = 1,
        err_socket_bind = 2,
        err_socket_listening = 3,
        close = 4
    };

private:
    uint16_t port; //Порт
    status _status = status::close;
    handler_function_t handler;
    std::thread handler_thread;
    std::list<std::thread> client_handler_threads;
    std::list<std::thread::id> client_handling_end;

    SOCKET serv_socket = INVALID_SOCKET;
    WSAData w_data;

    void handlingLoop();

public:
    TcpServer(const uint16_t port, handler_function_t handler);
    ~TcpServer();

    //! Set client handler
    void setHandler(handler_function_t handler);

    uint16_t getPort() const;
    uint16_t setPort(const uint16_t port);

    status getStatus() const { return _status; }

    status restart();
    status start();
    void stop();

    void joinLoop();
};

class TcpServer::Client {
    SOCKET socket;
    SOCKADDR_IN address;
    CommandData Test;
    Client* connected_to = nullptr;
    char buffer[buffer_size];
public:
    Client(SOCKET socket, SOCKADDR_IN address);
public:
    Client(const Client& other);
    ~Client();
    uint32_t getHost() const;
    uint16_t getPort() const;

    CommandOut outism;
    int loadData();
    char* getData();

    bool sendData(const char* buffer, const size_t size) const;
};

#endif // SSERVER_H