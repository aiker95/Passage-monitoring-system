
//#ifdef UF_CONF_USESTDAFX
#include "stdafx.h"
//#endif

#include <Windows.h>

#include <cstdlib>

#include "..\Framework\stuff.h"
#include "Port_RS232.h"
#include "log.h"

using namespace UFramework;

///////////////////////////////////////
/// Constructor
///////////////////////////////////////
CF_Port_RS232::CF_Port_RS232() : CF_Port( PT_RS232 )
{
	ZeroMemory(&m_timeouts, sizeof(COMMTIMEOUTS));
	m_hPort = INVALID_HANDLE_VALUE;
	SetPortParameters(
		Baud_9600,
		Bits_8,
		NoParity,
		StopBits_One);	
};

///////////////////////////////////////
/// Destructor
///////////////////////////////////////
CF_Port_RS232::~CF_Port_RS232() {
	Close();
};

///////////////////////////////////////
/// Opens COM Port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::Open(const std::string &portName)
 {
	CF_ErrorType error = CF_Port::Open(portName);
	if (error != ErrorOK) {
		return error;
	}

	/// Timeouts
	ZeroMemory(&m_timeouts,sizeof(COMMTIMEOUTS));
	/// Read...
	m_timeouts.ReadIntervalTimeout			= 300; // MAXDWORD;
	m_timeouts.ReadTotalTimeoutMultiplier	= 300; // MAXDWORD;
	m_timeouts.ReadTotalTimeoutConstant		= 0;//100;	// 300
	/// ... and Write Timeouts
	m_timeouts.WriteTotalTimeoutMultiplier = 300;
	m_timeouts.WriteTotalTimeoutConstant	= 0;//20000;
	
	int portNum = 0;
	if (   portName.length() > 3
		&& 'C' == portName[0] || 'c' == portName[0]
	    && 'O' == portName[1] || 'o' == portName[1]
	    && 'M' == portName[2] || 'm' == portName[2]
		){
			portNum = atoi(portName.c_str()+3);
		}
	if ( !portNum ) {
		return ERR_CreateInstance(ERR_INVALID_ARGUMENT, 0);
	}

	wchar_t wstrPort[64] = { 0 };
	//swprintf(wstrPort,  sizeof(wstrPort)/sizeof(wstrPort[0]), L"COM%d:", portNum);
	swprintf(wstrPort,  L"COM%d:", portNum);

	m_hPort = CreateFileW(
		wstrPort,	// widePortName,	// Port name
		GENERIC_READ | GENERIC_WRITE,	// Access
		0,								// No share
		NULL,							// No security attributes
		OPEN_EXISTING,					// Open existing COM-port
		0,								// No flags
		NULL							// No template file
		);
	//delete [] widePortName;
	if (m_hPort == INVALID_HANDLE_VALUE) {
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,GetLastError());
	}

	ZeroMemory(&m_portParams,sizeof(DCB));

	BOOL Ok = FALSE; /// Normal work flag for system functions;

	/// Getting port configuration
	Ok = GetCommState(m_hPort, &m_portParams);
	if (Ok == FALSE) {
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,GetLastError());
	}

	m_portParams.DCBlength			= sizeof( DCB );
	m_portParams.fBinary			= TRUE;
	m_portParams.BaudRate			= (DWORD)m_baudRate;
	m_portParams.ByteSize			= (BYTE)m_dataBits;
	m_portParams.Parity				= (BYTE)m_parity;
	//m_portParams.StopBits			= m_stopBits;
	m_portParams.fDsrSensitivity	= FALSE;
	m_portParams.fRtsControl		= RTS_CONTROL_DISABLE;
	m_portParams.fDtrControl		= DTR_CONTROL_DISABLE;
	m_portParams.fOutxCtsFlow		= FALSE;
	m_portParams.fOutxDsrFlow		= FALSE;
	m_portParams.fParity			= TRUE;
	m_portParams.fAbortOnError		= TRUE;


	CLogger lg( m_strPortName.c_str(), __FILE__, __LINE__  );
	

	/// Set port parameters
	Ok = SetCommState(m_hPort, &m_portParams);
	if (Ok == FALSE) {
		lg << " !!!! SetCommState ";
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED,GetLastError());
	}

	/// Set port input and output buffer size
	Ok = SetupComm( m_hPort, 2048, 2048 );
	if (Ok == FALSE) {
		lg << " !!!! SetupComm ";
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED,GetLastError());
	}

	/// Set timeouts
	Ok = SetCommTimeouts(m_hPort,&m_timeouts);
	if (Ok == FALSE) {
		lg << " !!!! SetCommTimeouts ";
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED,GetLastError());
	}

	/// Sets monitorable events
	Ok = SetCommMask(m_hPort, EV_RXCHAR | EV_CTS | EV_DSR | EV_RLSD | EV_BREAK | EV_ERR);
	if (Ok == FALSE) {
		lg << " !!!! SetCommMask ";
		return ERR_CreateInstance(ERR_PORT_OPEN_FAILED,GetLastError());
	}

	Ok = PurgeComm(m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	return ErrorOK;
};

///////////////////////////////////////
/// Sets port parameters. Must be used before open.
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::SetPortParameters(
	BaudRate	baudRate, 
	DataBits	dataBits,
	Parity    parity,
	StopBits  stopBits
	) {
		m_baudRate = baudRate;
		m_dataBits = dataBits;
		m_parity   = parity;
		m_stopBits = stopBits;

		return ErrorOK;
}

///////////////////////////////////////
/// Closes COM port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::Close() {
	CF_Port::Close();

	if (m_hPort != INVALID_HANDLE_VALUE) {
		///Discards all characters from the output or input buffer 
		PurgeComm(m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle( m_hPort );
		m_hPort=INVALID_HANDLE_VALUE;
	} else {
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);
	}

	return ErrorOK;
}

/*
CF_ErrorType CF_Port_RS232::Reopen()
{
	CF_ErrorType error = ErrorOK;
	

	m_portMutex.Wait();
	
	{
		std::string portname = m_strPortName;

		Close();
		error = Open(portname);
		if(error!=ErrorOK)
			error = ERR_CreateInstance(ERR_PORT_NOT_OPENED, GetLastError());
		else
			m_lastFailed = false;
	}
	
	m_portMutex.Release();

	

	return error;
}
*/

///////////////////////////////////////
/// Sends data to port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::Send( 
								 const unsigned char * data, 
								 unsigned int    dataSize, 
								 unsigned int  * dataSent, 
								 unsigned int    timeout 
								 ) 
{
	CF_ErrorType error = ErrorOK;
	if(error!=ErrorOK)
		return error;
	

	if (m_hPort == INVALID_HANDLE_VALUE) 
	{
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);	
	}
	//SetTimeOut( ~0, timeout);

	 
	BOOL Ok = FALSE; /// Normal work flag for system functions;
	 /// Writes data to file
	DWORD errorMask = 0;

	//PurgeComm(m_hPort, PURGE_TXABORT | PURGE_TXCLEAR );

	ClearCommError(m_hPort,&errorMask,NULL);

	Ok = WriteFile(m_hPort,   /// Port Handle
		 data,	  /// Data to be written
		 dataSize,  /// Number of bytes to be written
		 (LPDWORD)dataSent,  /// Number of bytes was written
		 NULL);

	if (FALSE == Ok) 
	{
		 m_lastFailed = true;
		 return ERR_CreateInstance(ERR_PORT_WRITE_ERROR, GetLastError());
	}
	m_lastFailed = false;

	CLogger lg( m_strPortName.c_str(), __FILE__, __LINE__  );
	lg << " send: \r\n";
	lg.HexToString(data, dataSize) << END;

	return ErrorOK;
};



///////////////////////////////////////
/// Receives data from port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::Receive( 
									unsigned char * data, 
									unsigned int    dataSize, 
									unsigned int  * dataReceived, 
									unsigned int    timeout 
									) 
{
	DWORD bytesRead = 0;
	*dataReceived = 0;
	CF_ErrorType error;

	// error = Reopen(); !!! What is the fuck???
	// May be That ->
	if(m_lastFailed==true)
	{
			return error;
	}


	if (m_hPort == INVALID_HANDLE_VALUE) {
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);
	}

	BOOL Ok = FALSE; /// Normal work flag for system functions;

	//SetTimeOut(timeout);

	DWORD startTime = StartTimer();

	while (1) {
		unsigned char byte = 0; /// Read this byte from port
		DWORD read = 0;			/// Number of bytes was read. Must be 1 or 0
		Ok = ReadFile(m_hPort,&byte, 1, &read, NULL);

		/// If no data from device - exit
		/*		if (Ok && (read == 0)) {			
		return ErrorOK;
		}*/

		/// If byte read
		if (read == 1) {
			*(data+bytesRead) = byte;
			bytesRead++;
			*dataReceived = bytesRead;
		}

		/// All data ready
		if (bytesRead >= dataSize) {
			m_lastFailed = false;
			CLogger lg( m_strPortName.c_str(), __FILE__, __LINE__  );
			lg << " recive: \r\n";
			lg.HexToString(data, dataSize) << END;

			return ErrorOK;
		}

		/// Error
		if (!Ok) 
		{
			m_lastFailed = true;
			return ERR_CreateInstance(ERR_PORT_READ_ERROR,GetLastError());
		}

		/// Timeout
		if (Timeout(timeout,startTime)) {
			m_lastFailed = false;
			return ERR_CreateInstance(ERR_TIMEOUT,0);
		}

		
	}

	m_lastFailed = false;
	return ErrorOK;
};

/// Receives data from port
/// @param[out] buffer    Array for received data
/// @param[out] dataReceived  Number of sent bytes
/// @param[in]  timeout   Timeout of operation
CF_ErrorType CF_Port_RS232::Receive( 
									CF_ByteArray &data,
									unsigned int dataToReceive,
									unsigned int timeout ) 
{


	UFramework::CF_ErrorType error = UFramework::ErrorOK;

	if (dataToReceive != 0) {
		unsigned char *buffer = new unsigned char[dataToReceive];
		unsigned int received = 0;
		error = Receive(buffer, dataToReceive, &received, timeout);
		data.assign(buffer, &buffer[received]);
		delete [] buffer;
		buffer = NULL;
	}

	return error;
}

///////////////////////////////////////
/// Receives data from port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::ReceiveFor( CF_ByteArray   &data,
										unsigned char	forThis, 
										unsigned int    timeout 
									   ) 
{
	DWORD bytesRead = 0;
	CF_ErrorType error;

	// error = Reopen(); !!! What is the fuck???
	// May be That ->
	if(m_lastFailed==true)
		return error;

	if (m_hPort == INVALID_HANDLE_VALUE)
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);

	BOOL Ok = FALSE; /// Normal work flag for system functions;
	DWORD startTime = StartTimer();

	while (1) 
	{
		unsigned char byte = 0; /// Read this byte from port
		DWORD read = 0;			/// Number of bytes was read. Must be 1 or 0
		Ok = ReadFile(m_hPort,&byte, 1, &read, NULL);
		
		/// If byte read
		if (read == 1)
			data.push_back(byte);

		/// All data ready
		if (byte == forThis)
		{
			m_lastFailed = false;
			return ErrorOK;
		}

		/// Error
		if (!Ok) 
		{
			m_lastFailed = true;
			return ERR_CreateInstance(ERR_PORT_READ_ERROR,GetLastError());
		}

		/// Timeout
		if ( Timeout(timeout,startTime) )
		{
			m_lastFailed = false;
			return ERR_CreateInstance(ERR_TIMEOUT,0);
		}	
	}

	m_lastFailed = false;
	return ErrorOK;
};


///////////////////////////////////////
/// Flushes all unread data from port
///////////////////////////////////////
CF_ErrorType CF_Port_RS232::Flush() {
	if (m_hPort == INVALID_HANDLE_VALUE) {
		return ERR_CreateInstance(ERR_PORT_NOT_OPENED,0);
	}

	BOOL Ok = FALSE; /// Normal work flag for system functions;
	///Discards all characters from the output or input buffer 
	Ok = PurgeComm(m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (Ok == FALSE) {
		return ERR_CreateInstance(ERR_PORT_ERROR,GetLastError());		
	}

	return ErrorOK;
};

///////////////////////////////////////
/// Set timeout params
///////////////////////////////////////
void CF_Port_RS232::SetTimeOut(unsigned int TimOutR, unsigned int TimOutW)
{


	bool bCange = false;

	if ( (TimOutR != (~0)) && (TimOutR != m_timeouts.ReadTotalTimeoutConstant) )
	{
		m_timeouts.ReadIntervalTimeout = ~0;
		m_timeouts.ReadTotalTimeoutMultiplier = 0;
		m_timeouts.ReadTotalTimeoutConstant = TimOutR;
		bCange = true;
	}

	if ( (TimOutW != (~0)) && (TimOutW != m_timeouts.WriteTotalTimeoutConstant) )
	{
		m_timeouts.WriteTotalTimeoutMultiplier = 0;
		m_timeouts.WriteTotalTimeoutConstant = TimOutW;
		bCange = true;
	} 

	if (bCange && (m_hPort != INVALID_HANDLE_VALUE))
		SetCommTimeouts(m_hPort, &m_timeouts);

	return;
}

/*
UFramework::BaudRate CF_Port_RS232::BaIntToBaudrate( int speed )
{
  BaudRate portSpeed;

  switch ( speed ) 
  {
  case 110 : portSpeed = Baud_110; break;	
  case 300 : portSpeed = Baud_300; break;	
  case 600 : portSpeed = Baud_600; break;	
  case 1200 : portSpeed = Baud_1200; break;	
  case 2400 : portSpeed = Baud_2400; break;	
  case 4800 : portSpeed = Baud_4800; break;	
  case 9600 : portSpeed = Baud_9600; break;	
  case 14400 : portSpeed = Baud_14400; break;
  case 19200 : portSpeed = Baud_19200; break;
  case 38400 : portSpeed = Baud_38400; break;
  case 57600 : portSpeed = Baud_57600; break;
  case 115200 : portSpeed = Baud_115200; break;
  case 128000 : portSpeed = Baud_128000; break;
  case 256000 : portSpeed = Baud_256000; break;
  default:
    portSpeed = Baud_unknown;
  }

  return portSpeed;
}
*/

bool CF_Port_RS232::IsDSR()
{
	return false;
}

CF_ErrorType CF_Port_RS232::SetControlLines( unsigned int enableDTR, unsigned int enableRTS )
{
	CF_ErrorType error = ErrorOK;
	m_portParams.fDtrControl = enableDTR;
	m_portParams.fRtsControl = enableRTS;
	if(SetCommState(m_hPort, &m_portParams)==FALSE)
	{
		error = ERR_CreateInstance(ERR_PORT_ERROR,0);
	}

	return error;
	
}
