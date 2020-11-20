//#ifdef UF_CONF_USESTDAFX
	#include "stdafx.h"
//#endif

#include "Port.h"
#include "Port_RS232.h"

//#include "tchar.h"
#include <Windows.h>


using namespace UFramework;
//////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////
UFramework::CF_Port::CF_Port(TPortType portType) {
  m_portType = portType;
};
//////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////
UFramework::CF_Port::~CF_Port() {
	Close();
};
//////////////////////////////////////////////////////////////////////////
/// Opens port for send-receive
/// @param strPortName Name of port to be opened
//////////////////////////////////////////////////////////////////////////
UFramework::CF_ErrorType UFramework::CF_Port::Open(const std::string &portName ) {

	CF_ErrorType error = ErrorOK;

	if (portName.length()==0)
	{
		error = ERR_CreateInstance( ERR_PORT_NOT_OPENED, ERR_INFO_NO );
	} 
	else 
	{
		error = Close();
		if(error==ErrorOK || error==ERR_CreateInstance(ERR_PORT_NOT_OPENED,0))	
		{
			error = ErrorOK;		
			m_strPortName = portName;			
		}
	}

	return error;
}
//////////////////////////////////////////////////////////////////////////
/// Closes port
//////////////////////////////////////////////////////////////////////////
UFramework::CF_ErrorType UFramework::CF_Port::Close() {
	
	m_strPortName.clear();

	return ErrorOK;
}

//////////////////////////////////////////////////////////////////////////
/// Starts timer of timeout
//////////////////////////////////////////////////////////////////////////
DWORD UFramework::CF_Port::StartTimer() {
	return GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
/// Check for timeout
//////////////////////////////////////////////////////////////////////////
BOOL UFramework::CF_Port::Timeout(DWORD timeout, DWORD startTime) {

	BOOL result = TRUE;

	DWORD currentTime = GetTickCount();

	if (( currentTime - startTime) <= timeout) {
		result = FALSE;
	} 
	
	return result;
}

/// Sends data to port
/// @param[in]  data      Data array to be sent
/// @param[out] dataSent  Number of sent bytes
/// @param[in]  timeout   Timeout of operation
CF_ErrorType CF_Port::Send(const CF_ByteArray &data, unsigned int *dataSent, unsigned int timeout) {
	return Send(&data[0], data.size(), dataSent, timeout);
}
