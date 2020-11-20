#ifndef SCLIENT_H
#define SCLIENT_H
#include "includes.h"

#define localhost 0x0100007f

typedef SOCKET socket_t;

#define buffer_size 4096


struct CommandData
{
	int Test1;
	int Test2;
	const char* start_time;
};

struct CommandInp
{
	int Status;
};

struct TcpClient {

	enum class status : uint8_t {
		connected = 0,
		err_socket_init = 1,
		err_socket_bind = 2,
		err_socket_connect = 3,
		disconnected = 1
	};

private:
	char buffer[buffer_size];
	status _status = status::disconnected;
	socket_t client_socket;
	WSAData w_data;
public:

	CommandInp inimi;
	TcpClient() noexcept;
	~TcpClient();

	status connectTo(uint32_t host, uint16_t port) noexcept;
	status disconnect() noexcept;

	uint32_t getHost() const;
	uint16_t getPort() const;

	status getStatus() {return _status;}

	int loadData();
	char* getData();

	bool sendData(const char* buffer, const size_t size) const;
};

#endif // SCLIENT_H