//#ifdef UF_CONF_USESTDAFX
	#include "stdafx.h"
//#endif
#include "Port_TCP.h"
#include "Port_TCPServer.h"

using namespace UFramework;

//int  instanses=0; in Port_TCP.h
//bool wsa_init=false; in Port_TCP.h

//////////////////////////////////////////////////////////////////////////
/// Server
//////////////////////////////////////////////////////////////////////////


CF_Port_TCPServer::CF_Port_TCPServer(unsigned int numPort) : m_numPort(numPort)
{
	m_sock=INVALID_SOCKET;
	instanses++;
  
  m_numPort = numPort;
	
  int wsaerr;

	if(wsa_init==false)
	{
		WORD wVersionRequested;
		WSADATA wsaData;


		wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		if(wsaerr==0)
			wsa_init=true;
		else
			lastError = ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0); 
	}

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP /*0*/);
	if (m_sock == INVALID_SOCKET)
	{
		lastError = ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0);
	}
	else
	{
		struct  sockaddr_in sin;

		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		//sin.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY;
		sin.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY;
		sin.sin_port = htons(m_numPort);
		wsaerr = bind(m_sock, (struct sockaddr *) &sin, sizeof(sin));
		if (wsaerr) {
			lastError = ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0);
		}
		else
		{
			wsaerr = listen(m_sock, QUEUE_SIZE);
			if (wsaerr) {
				lastError = ERR_CreateInstance(ERR_PORT_OPEN_FAILED, 0);
			}
			//else
			//{
			//  ULONG timeout = TIME_OUT;
			//  wsaerr = setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (const char*)&timeout, sizeof(timeout));
			//}
		}
	}
}

UFramework::CF_Port_TCPServer::~CF_Port_TCPServer()
{
	Close();
	instanses--;
	if(instanses==0 && wsa_init==true)
	{
		WSACleanup();
		wsa_init=false;
	}
}

CF_ErrorType UFramework::CF_Port_TCPServer::Close()
{
	if(m_sock!=INVALID_SOCKET)
	{
		closesocket(m_sock);
		m_sock=INVALID_SOCKET;
	}
	return ErrorOK;
}

CF_ErrorType UFramework::CF_Port_TCPServer::GetConection( CF_Port* &conectedPort, unsigned int timeout)
{
	int wsaerr;
	CF_ErrorType res = ErrorOK;

	conectedPort = NULL;

	if(lastError != ErrorOK )
		res = lastError;
	else
	{
		SOCKET sd;

		struct   sockaddr_in pin;
		int 	 addrlen;

		fd_set readset;
		FD_ZERO(&readset);
		FD_SET(m_sock, &readset);
		timeval timeout_select;
		timeout_select.tv_sec = timeout / 1000;
		timeout_select.tv_usec = (timeout % 1000) * 1000;
		wsaerr = select(1, &readset, NULL, NULL, &timeout_select);
		if(wsaerr == SOCKET_ERROR)
		{
			res = ERR_CreateInstance(ERR_NULL_POINTER, WSAGetLastError());
		}
		else if( (wsaerr == 0) || !FD_ISSET(m_sock, &readset) )
      res = ERR_CreateInstance(ERR_TIMEOUT,0);
    else
		{
			addrlen = sizeof(pin); 
      //pin.sin_port = 10310;
			sd = accept(m_sock, (struct sockaddr *)  &pin, &addrlen); 
      //sd = accept(m_sock, (struct sockaddr *)  NULL, NULL);
			if (sd == INVALID_SOCKET)
				res = ERR_CreateInstance(ERR_PORT_OPEN_FAILED, WSAGetLastError());
			else
			{
				conectedPort = (CF_Port*) new CF_Port_TCP(sd);
				if (conectedPort == NULL)
				{
					closesocket(sd);
					res = ERR_CreateInstance(ERR_NULL_POINTER, 0);
				}
      }
		}
	}

	return res;
}