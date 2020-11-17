#ifndef SSERVER_H
#define SSERVER_H
#include "includes.h"

//Буффер для приёма данных от клиента
static constexpr uint16_t buffer_size = 4096;

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

    status getStatus() const {return _status;}

    status restart();
    status start();
    void stop();

    void joinLoop();
};


class TcpServer::Client {
    SOCKET socket;
    SOCKADDR_IN address;
    Client* connected_to = nullptr;
    char buffer[buffer_size];
public:
    Client(SOCKET socket, SOCKADDR_IN address);
public:
    Client(const Client& other);
    ~Client();
    uint32_t getHost() const;
    uint16_t getPort() const;
    
	void connectTo(Client& other_client) const;
	void waitConnect() const;

    int loadData();
    char* getData();

    bool sendData(const char* buffer, const size_t size) const;
};

#endif // SSERVER_H