#ifndef _PORT_TCP_H_
#define _PORT_TCP_H_


// It neead be before other inludes becous may be conflick with <windows.h> include.
// You may use WIN32_LEAN_AND_MEAN definition in project property also.
#if WINVER <= 0x500 // WIN_CE
#include <winsock2.h>
#include <ws2tcpip.h>
#else				// _WIN32_WINNT
#include <ws2tcpip.h>
#endif

#include "common_includes.h"
#include "Port.h"
#include "ErrorType.h"


namespace UFramework
{
  extern int instanses;
  extern bool wsa_init;
	
  class CF_Port_TCP : public CF_Port
	{
    enum {TIME_OUT = 3000}; // in micro second
	public:		
		CF_Port_TCP(void);
		CF_Port_TCP(SOCKET hSocket );

		virtual ~CF_Port_TCP(void);

		/// Opens port for send-receive
		CF_ErrorType Open(const std::string &strPortName);
		CF_ErrorType Open(SOCKET socket);
		CF_ErrorType Open(DWORD address, USHORT port);

		bool CedeConnection(CF_Port_TCP &newKeeper)
		{
			if (INVALID_SOCKET == m_sock)
				return false;
			
			newKeeper.m_sock = m_sock;
			m_sock=INVALID_SOCKET;
			
			return  true;
		}

		/// Check port is open
		bool IsOpen(){ return m_sock == INVALID_SOCKET ? false : true; }
    
    /// Close port for send-receive
    CF_ErrorType Close();


		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[in]  dataSize  Size of data array
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		virtual CF_ErrorType Send(  const unsigned char * data, unsigned int dataSize,
									unsigned int * dataSent, unsigned int timeout);

		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		//virtual CF_ErrorType Send(	const CF_ByteArray &data,
		//							unsigned int *dataSent, 
		//							unsigned int timeout );

		/// Receives data from port
		/// @param[out]    data         Array for received data
		/// @param[in,out] dataSize     Size of array and number of data to be reiced
		/// @param[out]    dataReceived Number of received bytes
		/// @param[in]     timeout      Timeout of operation
		CF_ErrorType Receive(	unsigned char * data, 
								unsigned int    dataSize, 
								unsigned int  * dataReceived, 
								unsigned int    timeout	);


		/// Receives data from port
		/// @param[out] buffer    Array for received data
		/// @param[out] dataReceived  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		virtual CF_ErrorType Receive(   CF_ByteArray &data,
										unsigned int dataToReceive,
										unsigned int timeout );

		CF_ErrorType Flush();
	protected:
		SOCKET m_sock;
		//static int instanses;
		//static bool wsa_init;

		void SetKeepAlive(SOCKET s, int interval, int timeout);

	};

}

#endif