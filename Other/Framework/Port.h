#ifndef _PORT_H_
#define _PORT_H_

#include "common_includes.h"
#include "ErrorType.h"
#include "ascii.h"

namespace UFramework {
	
	/// Base class for ports of devices.
	class CF_Port
	{
	public:
    enum TPortType {PT_RS232, PT_TCP};
		TPortType   m_portType; // What Type of Port is created!
		
    CF_Port(TPortType portType);			/// Constructor		
		virtual ~CF_Port();	/// Destructor

		virtual CF_ErrorType Open(const std::string &portName);	/// Opens port for send-receive
    
    virtual bool IsOpen(){ return false; } /// Port is open or no.
		
    virtual CF_ErrorType Close();						/// Closes port
		
		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[in]  dataSize  Size of data array
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		virtual CF_ErrorType Send( const unsigned char * data, unsigned int dataSize, 
									unsigned int * dataSent, unsigned int timeout ) = 0;

		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		virtual CF_ErrorType Send( const CF_ByteArray &data, unsigned int *dataSent, unsigned int timeout );


		/// Receives data from port
		/// @param[out]    buffer         Array for received data
		/// @param[in,out] bufferSize     Size of array and number of data to be reiced
		/// @param[out]    dataReceived Number of received bytes
		/// @param[in]     timeout      Timeout of operation
		virtual CF_ErrorType Receive( unsigned char * buffer, unsigned int bufferSize, 
			unsigned int * dataReceived, unsigned int timeout ) = 0;

		/// Receives data from port
		/// @param[out] buffer    Array for received data
		/// @param[out] dataReceived  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		//virtual CF_ErrorType Receive( CF_ByteArray &data, unsigned int dataToReceive, unsigned int timeout ) = 0;

		/// Flushes all unread data from port
		virtual CF_ErrorType Flush() = 0;

	protected:
		unsigned long StartTimer();	/// Starts timer of timeout
		int  Timeout( unsigned long timeout, unsigned long startTime); /// Check for timeout

		std::string m_strPortName;	/// Name of port
	};
}

#endif