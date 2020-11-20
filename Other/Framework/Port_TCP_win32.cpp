//#ifdef UF_CONF_USESTDAFX
	#include "stdafx.h"
//#endif

#include "Port_TCP.h"
#include "log.h"


using namespace UFramework;
using std::string;

int  UFramework::instanses=0;
bool UFramework::wsa_init=false;

UFramework::CF_Port_TCP::CF_Port_TCP(void) : CF_Port(PT_TCP)
{
	m_sock=INVALID_SOCKET;
	instanses++;
}

UFramework::CF_Port_TCP::CF_Port_TCP( SOCKET hSocket ) : CF_Port(PT_TCP)
{
  int res=0;
  u_long iMode = 1;

  instanses++;

  if(hSocket==INVALID_SOCKET )
  {
    m_sock=INVALID_SOCKET;
    return; // ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0);
  }
  else
    m_sock = hSocket;

  ioctlsocket(m_sock, FIONBIO, &iMode);
  return; // ErrorOK;
}

UFramework::CF_Port_TCP::~CF_Port_TCP(void)
{
	Close();
	instanses--;
	if(instanses==0 && wsa_init==true)
	{
		WSACleanup();
		wsa_init=false;
	}
}


UFramework::CF_ErrorType UFramework::CF_Port_TCP::Open(const std::string &strPortName )
{
	UFramework::CF_ErrorType error = CF_Port::Open(strPortName);
	if (error != ErrorOK) {
		return error;
	}
	

	WORD wVersionRequested;
	WSADATA wsaData;
	int wsaerr;
	int res=0;

	//struct addrinfo aiHints;
  
	//struct addrinfo *aiList = NULL;
	addrinfo* aiList = NULL;	
	
	u_long iMode = 1;
	


	// Using MAKEWORD macro, Winsock version request 2.2

	if(wsa_init==false)
	{
		wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		if(wsaerr==0)
			wsa_init=true;
		else
			return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, WSAGetLastError()); 
	}
	
	static const std::string SCHEMA_PREFIX("://"); // For example the http schema is "http://"
	std::size_t  fndPos = strPortName.find(SCHEMA_PREFIX);
	string serv_name(strPortName, (string::npos == fndPos) ? 0 : fndPos + SCHEMA_PREFIX.length());
	
	fndPos = serv_name.find_last_of('/');
	if (string::npos != fndPos)
	{
		return ERR_CreateInstance(ERR_INVALID_ARGUMENT, 0);
	}

	string port;
	string addr;

	fndPos = serv_name.find_last_of(':');
	if (string::npos == fndPos)
	{
		port="80"; // Default for http schemes
		addr.swap(serv_name);
	}
	else
	{
		port.swap(serv_name.substr(fndPos + 1));
		serv_name.resize(fndPos);
		addr.swap(serv_name);
	}

	if(m_sock==INVALID_SOCKET)
	{
		m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(m_sock==INVALID_SOCKET )
		{
			return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, WSAGetLastError());
		}

	}

	//memset(&aiHints, 0, sizeof(aiHints));
	//aiHints.ai_family = AF_UNSPEC;
	//aiHints.ai_socktype = SOCK_STREAM;
	//aiHints.ai_protocol = IPPROTO_TCP;

	res = getaddrinfo(addr.c_str(), port.c_str(), NULL, &aiList);

	if(res!=0)
	{
		res = WSAGetLastError();
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, res);
	}

	res=connect(m_sock,aiList->ai_addr,aiList->ai_addrlen);
	if(res==SOCKET_ERROR)
	{
		res = WSAGetLastError();
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;
		freeaddrinfo(aiList);
		
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, res);
	}

	freeaddrinfo(aiList);

	int iOptVal = TIME_OUT;
	int iOptLen = sizeof(int);
	setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&iOptVal,iOptLen);

	ioctlsocket(m_sock, FIONBIO, &iMode);
	return ErrorOK;
}

UFramework::CF_ErrorType UFramework::CF_Port_TCP::Open(DWORD address, USHORT port)
{
	UFramework::CF_ErrorType error = UFramework::ErrorOK;

	WORD wVersionRequested;
	WSADATA wsaData;
	int wsaerr;
	int res=0;

	u_long iMode = 1;



	// Using MAKEWORD macro, Winsock version request 2.2

	if(wsa_init==false)
	{
		wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		if(wsaerr==0)
			wsa_init=true;
		else
			return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, WSAGetLastError()); 
	}


	if(m_sock==INVALID_SOCKET)
	{
		m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(m_sock==INVALID_SOCKET )
		{
			return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, WSAGetLastError());
		}

		//SetKeepAlive(m_sock,1000,10000);
	}

	sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = htonl( address );
	clientService.sin_port = htons( port );
	
	res=connect(m_sock,(SOCKADDR*) &clientService, sizeof(clientService) );
	if(res==SOCKET_ERROR)
	{
		res = WSAGetLastError();
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;

		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, res);
	}

	int iOptVal = TIME_OUT;
	int iOptLen = sizeof(int);
	setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&iOptVal,iOptLen);

	ioctlsocket(m_sock, FIONBIO, &iMode);
	return ErrorOK;
}

UFramework::CF_ErrorType UFramework::CF_Port_TCP::Open( unsigned int socket )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int wsaerr;
	int res=0;
	
	u_long iMode = 1;

	if(wsa_init==false)
	{
		wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		if(wsaerr==0)
			wsa_init=true;
		else
			return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0); 
	}


	if(socket==INVALID_SOCKET )
	{
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0);
	}
	m_sock = (SOCKET)socket;

	//SetKeepAlive(m_sock,1000,10000);

	int iOptVal = TIME_OUT;
	int iOptLen = sizeof(int);
	setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&iOptVal,iOptLen);

	ioctlsocket(m_sock, FIONBIO, &iMode);
	return ErrorOK;
}


UFramework::CF_ErrorType UFramework::CF_Port_TCP::Close()
{
	if(m_sock!=INVALID_SOCKET)
	{
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;
	}
	return ErrorOK;
}

CF_ErrorType CF_Port_TCP::Send(const unsigned char * data, unsigned int dataSize, unsigned int * dataSent, unsigned int timeout)
{
	(*dataSent)=0;
	if(m_sock==INVALID_SOCKET)
	{
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);
	}

	int res = 0;

	DWORD startTime = StartTimer();
	fd_set writeset;
	FD_ZERO(&writeset);
	FD_SET(m_sock, &writeset);
	timeval timeout_select;
	timeout_select.tv_sec = timeout / 1000;
	timeout_select.tv_usec = (timeout % 1000) * 1000;
	while(1)
	{
		res = send(m_sock,(char*)data,dataSize,0);

		if (res != SOCKET_ERROR)
		{
			CLogger lg( m_strPortName.c_str(), __FILE__, __LINE__  );
			lg << " send: \r\n";
			lg.HexToString(data, res) << END;

			dataSize-=res;
			data+=res;
			(*dataSent)+=res;

			if(dataSize<=0)
				break;
		}
		else
		{
			res = WSAGetLastError();
			switch(res)
			{
			case WSAEWOULDBLOCK:
				break;
			case WSAENETDOWN:
			case WSAENETRESET:
			case WSAECONNRESET:
			case WSAENOTCONN:
			case WSAETIMEDOUT:
				return ERR_CreateInstance(ERR_PORT_WRITE_ERROR,res);
			default:
				return ERR_CreateInstance(ERR_SYSTEM_ERROR, res);
			}
		}

		timeout_select.tv_sec = timeout / 1000;
		timeout_select.tv_usec = (timeout % 1000) * 1000;
		res = select(1, NULL, &writeset, NULL, &timeout_select);

		if(res == SOCKET_ERROR) {
			return ERR_CreateInstance(ERR_PORT_WRITE_ERROR, WSAGetLastError());
		}
		else if( (res == 0) || !FD_ISSET(m_sock, &writeset) )
			return ERR_CreateInstance(ERR_TIMEOUT, 0);


		if(Timeout(timeout,startTime))
		{
			return ERR_CreateInstance(ERR_TIMEOUT,res);
		}
		else
			timeout = startTime + timeout - StartTimer();
		//Sleep(5);
	}

	return ErrorOK;
}


UFramework::CF_ErrorType UFramework::CF_Port_TCP::Receive( unsigned char * data, unsigned int dataSize, unsigned int * dataReceived, unsigned int timeout )
{
	(*dataReceived) = 0;

	if (m_sock == INVALID_SOCKET)
	{
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED, 0);
	}

	int res = 0;


	DWORD startTime = StartTimer();
	fd_set readset;
	fd_set errorset;
	timeval timeout_select;

	FD_ZERO(&readset);
	FD_ZERO(&errorset);
	FD_SET(m_sock, &readset);
	FD_SET(m_sock, &errorset);
	while (1)
	{
		timeout_select.tv_sec = timeout / 1000;
		timeout_select.tv_usec = (timeout % 1000) * 1000;

		res = select(1, &readset, NULL, &errorset, &timeout_select);

		if (res == SOCKET_ERROR) {
			return ERR_CreateInstance(ERR_PORT_READ_ERROR, WSAGetLastError());
		}
		else if ((res == 0))//|| !FD_ISSET(m_sock, &readset) )
			return ERR_CreateInstance(ERR_TIMEOUT, 0);
		else
		{
			if (FD_ISSET(m_sock, &readset))
				res = recv(m_sock, (char*)data, dataSize, 0);
			else
				res = SOCKET_ERROR;

			if (res == SOCKET_ERROR)
			{
				res = WSAGetLastError();
				switch (res)
				{
					case WSAEWOULDBLOCK:
						break;
					case WSAENETDOWN:
					case WSAENETRESET:
					case WSAECONNRESET:
					case WSAENOTCONN:
					case WSAETIMEDOUT:
						return ERR_CreateInstance(ERR_PORT_READ_ERROR, res);
					default:
						return ERR_CreateInstance(ERR_SYSTEM_ERROR, res);
				}
			}
			else if (res == 0)
			{
				return ERR_CreateInstance(ERR_PORT_DISCONNECT, 0);
			}
			else
			{
				CLogger lg( m_strPortName.c_str(), __FILE__, __LINE__  );
				lg << " recive: \r\n";
				lg.HexToString(data, res) << END;


				dataSize -= res;
				data += res;
				(*dataReceived) += res;

				if (dataSize <= 0)
					break;
			}

			if (Timeout(timeout, startTime))
				return ERR_CreateInstance(ERR_TIMEOUT, 0);
			else
				timeout = startTime + timeout - StartTimer();
			//Sleep(5);
		}
	}

	return ErrorOK;
}

/// Receives data from port
/// @param[out] buffer    Array for received data
/// @param[out] dataReceived  Number of sent bytes
/// @param[in]  timeout   Timeout of operation
UFramework::CF_ErrorType UFramework::CF_Port_TCP::Receive( 
				CF_ByteArray &data,
				unsigned int dataToReceive,
				unsigned int timeout ) 
{


	UFramework::CF_ErrorType error = UFramework::ErrorOK;

	if (dataToReceive != 0) {
		data.resize(dataToReceive);
		unsigned int received = 0;
		error = Receive(&data[0], dataToReceive, &received, timeout);
		data.resize(received);
	}	
	return error;
}

UFramework::CF_ErrorType UFramework::CF_Port_TCP::Flush()
{
	return ErrorOK;
}

void UFramework::CF_Port_TCP::SetKeepAlive( SOCKET s, int interval, int timeout )
{

/*	tcp_keepalive alive;
	DWORD dwRet, dwSize;

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen);

	alive.onoff = 1;
	alive.keepalivetime = timeout;
	alive.keepaliveinterval = interval;

	dwRet = WSAIoctl(s, SIO_KEEPALIVE_VALS, &alive, sizeof(alive),
		NULL, 0, reinterpret_cast<DWORD*>(&dwSize), NULL, NULL);
*/
}