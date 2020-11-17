#include "includes.h"

class SClient
{
public:
	SClient(SOCKET s, SOCKADDR_IN sock_in);
	~SClient();
	void handle();
private:
	SOCKET c_sock;
	SOCKADDR_IN c_addr;
	char buffer[1024];
};

SClient::SClient(SOCKET s, SOCKADDR_IN sock_in)
{
	c_sock = s;
	c_addr = sock_in;
	printf("Client created\n");
	SClient::handle();
}


SClient::~SClient()
{
}

void SClient::handle()
{
	while (true)
	{
		int k = recv(c_sock, buffer, sizeof(buffer), NULL);
		if (k > 0) {
			printf(buffer);//Обработка полученных данных
		}
		Sleep(30);
	}
}
