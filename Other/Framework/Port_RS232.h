#ifndef _UNITELLER_PORT_RS232_
#define _UNITELLER_PORT_RS232_


#include "common_includes.h"
#include "port.h"
#include "ascii.h"

#ifdef USE_MFC
#include <afxwin.h>
#else
#include <Windows.h>
#endif
namespace UFramework {

	enum BaudRate{
		Baud_unknown = -1,
		Baud_110	= CBR_110,
		Baud_300	= CBR_300,
		Baud_600	= CBR_600,
		Baud_1200	= CBR_1200,
		Baud_2400	= CBR_2400,
		Baud_4800	= CBR_4800,
		Baud_9600	= CBR_9600,
		Baud_14400	= CBR_14400,
		Baud_19200	= CBR_19200,
		Baud_38400	= CBR_38400,
		Baud_57600	= CBR_57600,
		Baud_115200	= CBR_115200,
		Baud_128000	= CBR_128000,
		Baud_256000	= CBR_256000
	};

	enum Parity{
		NoParity = NOPARITY,
		Even     = EVENPARITY,
		Odd      = ODDPARITY,
		Mark     = MARKPARITY,
		Space    = SPACEPARITY
	};


	enum DataBits{
		Bits_5		= DATABITS_5,
		Bits_6		= DATABITS_6,
		Bits_7		= DATABITS_7,
		Bits_8		= DATABITS_8,
		Bits_16		= DATABITS_16,
		Bits_16x	= DATABITS_16X,
	};


	enum StopBits{
		StopBits_One  = STOPBITS_10,
		StopBits_One5 = STOPBITS_15,
		StopBits_Two  = STOPBITS_20
	};



	class CF_Port_RS232 : public CF_Port
	{
	public:
		CF_Port_RS232();
		virtual ~CF_Port_RS232();

		/// Opens port for send-receive
        CF_ErrorType Open(const std::string &portName);

		/// Check port is open
		bool IsOpen(){ return m_hPort == INVALID_HANDLE_VALUE ? false : true; }

		/// Closes port
		CF_ErrorType Close();

		//CF_ErrorType Reopen();

		/// Sets port parameters. Must be used before open.
		CF_ErrorType SetPortParameters(
		BaudRate	baudRate, 
		DataBits	dataBits,
		Parity    parity,
		StopBits  stopBits
		);

		CF_ErrorType SetControlLines(unsigned int enableDTR, unsigned int enableRTS);


		/* CF_ErrorType ReceiveByte( unsigned char * data, 
							unsigned int  * dataSent, 
							unsigned int    timeout 
		);*/

		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[in]  dataSize  Size of data array
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		CF_ErrorType Send( const unsigned char * data, 
							unsigned int    dataSize, 
		    				unsigned int  * dataSent, 
		    				unsigned int    timeout 
							);

		/// Sends data to port
		/// @param[in]  data      Data array to be sent
		/// @param[out] dataSent  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		//virtual CF_ErrorType Send( const CF_ByteArray &data, 
		//							unsigned int *dataSent, 
		//							unsigned int timeout
		//							);

		/// Receives data from port
		/// @param[out]    data         Array for received data
		/// @param[in,out] dataSize     Size of array and number of data to be reiced
		/// @param[out]    dataReceived Number of received bytes
		/// @param[in]     timeout      Timeout of operation
		CF_ErrorType Receive(  unsigned char * data, 
							   unsigned int    dataSize, 
							   unsigned int  * dataReceived, 
							   unsigned int    timeout
							);

		/// Receives data from port
		/// @param[out] buffer    Array for received data
		/// @param[out] dataReceived  Number of sent bytes
		/// @param[in]  timeout   Timeout of operation
		virtual CF_ErrorType Receive(   CF_ByteArray &data,
										unsigned int dataToReceive,
										unsigned int timeout );

		virtual CF_ErrorType ReceiveFor( CF_ByteArray   &data,
										 unsigned char	forThis,
										 unsigned int    timeout );

		/// Flushes all unread data from port
		CF_ErrorType Flush();

		void SetTimeOut(unsigned int TimOutR = (~0), unsigned int TimOutW = (~0));

		static BaudRate BaIntToBaudrate(int portSpeed);

		bool IsDSR();

		HANDLE getHandle() { return m_hPort; }


	protected:
		/// Handle of rs232 port
		HANDLE m_hPort;
		COMMTIMEOUTS	m_timeouts;		/// RS232 Timeouts
		DCB				m_portParams;	/// RS232 Port parameters

		/// Parameters from SetPortParameters
		BaudRate	 m_baudRate;
		DataBits	 m_dataBits;
		Parity     m_parity;
		StopBits   m_stopBits;
    
		/// Ставится в true, если последняя операция с портом 
		/// прошла с ошибкой. Требуется для переоткрытия порта
		bool m_lastFailed;

	};

}

#endif
