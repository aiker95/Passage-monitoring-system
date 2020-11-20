#ifndef _PORT_TCP_SERVER_H_
#define _PORT_TCP_SERVER_H_


#include <ws2tcpip.h>
#include "common_includes.h"
#include "Port.h"
#include "ErrorType.h"

namespace UFramework
{
//	static int instanses;
//	static bool wsa_init;

class CF_Port_TCPServer
{
	public:
		enum {TIME_OUT = 3, QUEUE_SIZE = 5};
		
		CF_Port_TCPServer(unsigned int numPort = 10300);
		virtual ~CF_Port_TCPServer(void);
		CF_ErrorType Close();
		CF_ErrorType GetConection( CF_Port* &conectedPort, unsigned int timeout = TIME_OUT );
		CF_ErrorType GetLastError(){ return lastError; }

	protected:
		SOCKET m_sock;
		CF_ErrorType lastError;
		unsigned int  m_numPort;
};

}


#endif