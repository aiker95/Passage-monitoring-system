#ifndef _ERROR_DEFINES_H_
#define _ERROR_DEFINES_H_

//////////////////////////////////////////////////////////////////////////
/// Error Codes
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
/// Error Innfo to detail the erroro code
//------------------------------------------------------------------------
#define ERR_INFO_NO 0x00	/// No ErrorInfo
#define ERR_INFO_BAD_SIZE    0x01
#define ERR_INFO_BAD_DATA    0x02
#define ERR_INFO_CANCELED    0xFF  

//------------------------------------------------------------------------
/// Common errors
//------------------------------------------------------------------------
#define ERR_COMMON_MASK		0x00000000
#define ERR_COMMON(error_code) ((error_code)|(ERR_COMMON_MASK))
//------------------------------------------------------------------------
#define ERR_OK                          ERR_COMMON(0x00000000)	/// No Error
#define ERR_NOT_IMPLEMENTED				ERR_COMMON(0x00000001)	/// Command is Not implemented
#define ERR_NULL_POINTER                ERR_COMMON(0x00000002)	/// Null pointer
#define ERR_LOW_MEMORY                  ERR_COMMON(0x00000003)	/// Low memory
#define ERR_INVALID_ARGUMENT            ERR_COMMON(0x00000004)	/// Invalid argument
#define ERR_SYSTEM_ERROR                ERR_COMMON(0x00000005)	/// System Error
#define ERR_NOT_INITIALISED             ERR_COMMON(0x00000006)	/// Class not initialized
#define ERR_TIMEOUT                     ERR_COMMON(0x00000007)	/// Timeout of operation
#define ERR_FAILURE                     ERR_COMMON(0x00000008)	/// Some error
#define ERR_INVALID_CONFIG              ERR_COMMON(0x00000009)	/// Invalid parameters in  config or not config
#define ERR_DEVICE                      ERR_COMMON(0x0000000A) // Any errors issuer in device
#define ERR_COMAND_NOT_POSSIBLE			ERR_COMMON(0x0000000B) // Command is not possible 
#define ERR_DEVICE_NOT_FOUND			ERR_COMMON(0x0000000C) // Device wasn`t found
#define ERR_DEVICE_IN_USE				ERR_COMMON(0x0000000D) // Device is in use
#define ERR_DEVICE_BUSY                 ERR_COMMON(0x0000000E) // Device is in use
#define ERR_FORMAT_ERROR				ERR_COMMON(0x0000000F)	// Error in format
#define ERR_PROCESS_BROKEN				ERR_COMMON(0x00000010)	// Some process was broken.

//------------------------------------------------------------------------
/// Port errors
//------------------------------------------------------------------------
#define ERR_PORT_MASK			0x00010000
#define ERR_PORT(error_code) ((error_code)|(ERR_PORT_MASK))
//------------------------------------------------------------------------
#define ERR_PORT_ERROR         ERR_PORT(0x00000000)	/// Port Error
#define ERR_PORT_OPEN_FAILED	ERR_PORT(0x00000001)	/// Can't open port
#define ERR_PORT_NOT_OPENED	ERR_PORT(0x00000002)	/// Port not opened
#define ERR_PORT_WRITE_ERROR	ERR_PORT(0x00000003)	/// Port write error
#define ERR_PORT_READ_ERROR	ERR_PORT(0x00000004)	/// Port read error
#define ERR_PORT_DISCONNECT	ERR_PORT(0x00000005)	/// Port disconnected
#define ERR_PORT_NAK			ERR_PORT(0x00000006)	/// NAK
//------------------------------------------------------------------------
/// Byte Data errors
//------------------------------------------------------------------------
#define ERR_BYTEARRAY_MASK		0x00020000
#define ERR_BYTEARRAY(error_code) ((error_code)|(ERR_BYTEARRAY_MASK))
//------------------------------------------------------------------------
#define ERR_BYTEARRAY_INDEX_OUT_OF_BOUND	ERR_BYTEARRAY(0x00000001)	/// Index in bytearray is out of bound
#define ERR_BYTEARRAY_NO_INPUT_DATA		ERR_BYTEARRAY(0x00000002)	/// No input data for bytearray
#define ERR_BYTEARRAY_NO_DATA				ERR_BYTEARRAY(0x00000003)	/// No data in object
#define ERR_BYTEARRAY_INVALID_FORMAT		ERR_BYTEARRAY(0x00000004)	/// Data has invalid format
#define ERR_BYTEARRAY_MODIFY_END_OF_PARAMS	ERR_BYTEARRAY(0x00000005)	/// Data has invalid format
//------------------------------------------------------------------------
//	Protocol errors
//------------------------------------------------------------------------
#define ERR_PROTOCOL_MASK		0x00040000
#define ERR_PROTOCOL(error_code) ((error_code)|(ERR_PROTOCOL_MASK))
//------------------------------------------------------------------------
#define ERR_PROTOCOL_FORMAT_ERROR				ERR_PROTOCOL(0x00000001)	/// Protocol command parse error in format
#define ERR_PROTOCOL_CRC_ERROR					ERR_PROTOCOL(0x00000002)	/// Protocol command parse error in CRC
#define ERR_PROTOCOL_CREATE_BYTE_ARRAY_FAILED	ERR_PROTOCOL(0x00000003)	/// Protocol command create array from command error
#define ERR_PROTOCOL_INVALID_SEQUENCE          ERR_PROTOCOL(0x00000004)   /// Protocol command recive invalid sequence
//------------------------------------------------------------------------
/// OS dependent functions errors
//------------------------------------------------------------------------
#define ERR_OS_DEPEND_MASK		0x00050000
#define ERR_OS_DEPEND(error_code) ((error_code)|(ERR_OS_DEPEND_MASK))
//------------------------------------------------------------------------
#define ERR_OS_DEPEND_NOT_IMPLEMENTED	ERR_OS_DEPEND(0x00000001)	/// OS Dependent function not implemented
#define ERR_OS_DEPEND_FAILED			ERR_OS_DEPEND(0x00000002)	/// OS Dependent function call failed
//------------------------------------------------------------------------
/// Synchronization object errors
//------------------------------------------------------------------------
#define ERR_SYNC_OBJ_MASK		0x00060000
#define ERR_SYNC_OBJ(error_code) ((error_code)|(ERR_SYNC_OBJ_MASK))
//------------------------------------------------------------------------
#define ERR_SYNC_OBJ_ABANDONED		ERR_SYNC_OBJ(0x00000002)		/// Synchronization object abandoned
#define ERR_SYNC_OBJ_CREATE		ERR_SYNC_OBJ(0x00000003)		/// Error creating synchronization object
//------------------------------------------------------------------------
/// Thread errors
//------------------------------------------------------------------------
#define ERR_THREAD_MASK		0x00080000
#define ERR_THREAD(error_code) ((error_code)|(ERR_THREAD_MASK))
//------------------------------------------------------------------------
#define ERR_THREAD_ALREADY_RUNNING	ERR_THREAD(0x00000001)		/// Thread is already running
#define ERR_THREAD_NOT_RUNNING		ERR_THREAD(0x00000002)		/// Thread is not running
#define ERR_THREAD_WAS_NOT_RUN		ERR_THREAD(0x00000003)		/// Thread was not run
#define ERR_THREAD_START				ERR_THREAD(0x00000004)		/// Cannot start thread
#define ERR_THREAD_SUSPEND_FAILED	ERR_THREAD(0x00000005)		/// Cannot suspend thread
#define ERR_THREAD_RESUME_FAILED	ERR_THREAD(0x00000006)		/// Cannot resume thread
#define ERR_THREAD_ILLEGAL_CONTROL ERR_THREAD(0x00000007)		/// Cannot call start/wait from the same thread
//------------------------------------------------------------------------
/// Dll Library errors
//------------------------------------------------------------------------
#define ERR_LIBRARY_MASK		0x00090000
#define ERR_LIBRARY(error_code) ((error_code)|(ERR_LIBRARY_MASK))
//------------------------------------------------------------------------
#define ERR_LIBRARY_SYSTEM_ERROR		ERR_LIBRARY(0x00000001)		/// Failed to load library
#define ERR_LIBRARY_FUNCTION_NOT_FOUND ERR_LIBRARY(0x00000002)		/// Function not found in library
#define ERR_LIBRARY_NOT_LOADED			ERR_LIBRARY(0x00000003)		/// Library not loaded

//------------------------------------------------------------------------
/// XML errors
//------------------------------------------------------------------------
#define ERR_XML_MASK		0x000A0000
#define ERR_XML(error_code) ((error_code)|(ERR_XML_MASK))
//------------------------------------------------------------------------
#define ERR_XML_SYNTAX_ERROR		ERR_XML(0x00000001)		/// переданный файл содержит синтаксическую ошибку
#define ERR_XML_SYNTAX_EXCEPTION	 ERR_XML(0x00000002)		/// прочитанный тег является служебным, возможно продолжение работы

//------------------------------------------------------------------------
/// File I/O errors
//------------------------------------------------------------------------
#define ERR_FILE_MASK		0x000B0000
#define ERR_FILE(error_code) ((error_code)|(ERR_FILE_MASK))
//------------------------------------------------------------------------
#define ERR_FILE_ERROR			ERR_FILE(0x00000001)		/// ошибка чтения/записи
#define ERR_FILE_NOT_FOUND		ERR_FILE(0x00000002)		/// файл не найден
#define ERR_FILE_EOF			ERR_FILE(0x00000003)		/// конец файла (End Of File)
//------------------------------------------------------------------------
/// Queue errors
//------------------------------------------------------------------------
#define ERR_QUEUE_MASK		0x000C0000
#define ERR_QUEUE(error_code) ((error_code)|(ERR_QUEUE_MASK))
//------------------------------------------------------------------------
#define ERR_QUEUE_EMPTY			ERR_QUEUE(0x00000001)		/// очередь пустая
//------------------------------------------------------------------------
/// Connection errors
//------------------------------------------------------------------------
#define ERR_CONNECTION_MASK		0x000D0000
#define ERR_CONNECTION(error_code) ((error_code)|(ERR_CONNECTION_MASK))
//------------------------------------------------------------------------
#define ERR_CONNECTION_ANSWER_OBSOLETE	ERR_CONNECTION(0x00000001)		/// Answer to service command is obsolete
#define ERR_CONNECTION_NOT_CONNECTED	ERR_CONNECTION(0x00000002)		/// Not connected to server

//------------------------------------------------------------------------
/// User errors
//------------------------------------------------------------------------
#define ERR_USER_MASK		0x80000000
#define ERR_USER(error_id,error_code) ((error_code)|(error_id << 16)|(ERR_USER_MASK))
//------------------------------------------------------------------------



/*

//////////////////////////////////////////////////////////////////////////
/// Error info
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#define ET_INFO_NO 0x00	/// No ErrorInfo
#define ET_INFO_BAD_SIZE    0x01
#define ET_INFO_BAD_DATA    0x02
#define ET_INFO_CANCELED    0xFF    

// Информация об ошибках, связанная с соединением с сервером
// во время ошибки при связи с сервером возникла неопределенность со статусом оплаты
#define ET_INFO_RESULT_UNPREDICTED 0x01

//------------------------------------------------------------------------
/// ERR_NULL_POINTER
//------------------------------------------------------------------------
#define ET_INFO_1_Parameter 0x00000001
#define ET_INFO_2_Parameter 0x00000002
#define ET_INFO_3_Parameter 0x00000003
#define ET_INFO_4_Parameter 0x00000004
#define ET_INFO_5_Parameter 0x00000005

//------------------------------------------------------------------------
/// Common errors
//------------------------------------------------------------------------
#define ET_ERROR_COMMON_MASK		0x00000000
#define ET_ERROR_COMMON(error_code) ((error_code)|(ET_ERROR_COMMON_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_OK					ET_ERROR_COMMON(0x00000000)	/// No Error
#define ET_ERROR_NOT_IMPLEMENTED	ET_ERROR_COMMON(0x00000001)	/// Command is Not implemented
#define ET_ERROR_NULL_POINTER		ET_ERROR_COMMON(0x00000002)	/// Null pointer   
#define ET_ERROR_LOW_MEMORY			ET_ERROR_COMMON(0x00000003)	/// Low memory   
#define ET_ERROR_INVALID_ARGUMENT	ET_ERROR_COMMON(0x00000004)	/// Invalid argument
#define ET_ERROR_SYSTEM_ERROR		ET_ERROR_COMMON(0x00000005)	/// System Error
#define ET_ERROR_NOT_INITIALISED	ET_ERROR_COMMON(0x00000006)	/// Class not initialized
#define ET_ERROR_TIMEOUT			ET_ERROR_COMMON(0x00000007)	/// Timeout of operation
#define ET_ERROR_FAILURE			ET_ERROR_COMMON(0x00000008)	/// Some error
#define ET_ERROR_INVALID_CONFIG			ET_ERROR_COMMON(0x00000009)	/// Invalid parameters in  config or not config
#define ET_ERROR_DEVICE			ET_ERROR_COMMON(0x0000000A) // Any errors issuer in device
#define ET_ERROR_COMAND_NOT_POSSIBLE			ET_ERROR_COMMON(0x0000000B) // Command is not possible 
#define ET_ERROR_DEVICE_NOT_FOUND			ET_ERROR_COMMON(0x0000000C) // Device wasn`t found
#define ET_ERROR_DEVICE_IN_USE				ET_ERROR_COMMON(0x0000000D) // Device is in use
#define ET_ERROR_DEVICE_BUSY				ET_ERROR_COMMON(0x0000000E) // Device is in use
#define ET_ERROR_FORMAT_ERROR				ET_ERROR_COMMON(0x0000000F)	// Error in format

//------------------------------------------------------------------------
/// Port errors
//------------------------------------------------------------------------
#define ET_ERROR_PORT_MASK			0x00010000
#define ET_ERROR_PORT(error_code) ((error_code)|(ET_ERROR_PORT_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_PORT_ERROR         ET_ERROR_PORT(0x00000000)	/// Port Error
#define ET_ERROR_PORT_OPEN_FAILED	ET_ERROR_PORT(0x00000001)	/// Can't open port
#define ET_ERROR_PORT_NOT_OPENED	ET_ERROR_PORT(0x00000002)	/// Port not opened
#define ET_ERROR_PORT_WRITE_ERROR	ET_ERROR_PORT(0x00000003)	/// Port write error
#define ET_ERROR_PORT_READ_ERROR	ET_ERROR_PORT(0x00000004)	/// Port read error
#define ET_ERROR_PORT_DISCONNECT	ET_ERROR_PORT(0x00000005)	/// Port disconnected
#define ET_ERROR_PORT_NAK			ET_ERROR_PORT(0x00000006)	/// NAK
//------------------------------------------------------------------------
/// Byte Data errors
//------------------------------------------------------------------------
#define ET_ERROR_BYTEARRAY_MASK		0x00020000
#define ET_ERROR_BYTEARRAY(error_code) ((error_code)|(ET_ERROR_BYTEARRAY_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_BYTEARRAY_INDEX_OUT_OF_BOUND	ET_ERROR_BYTEARRAY(0x00000001)	/// Index in bytearray is out of bound
#define ET_ERROR_BYTEARRAY_NO_INPUT_DATA		ET_ERROR_BYTEARRAY(0x00000002)	/// No input data for bytearray
#define ET_ERROR_BYTEARRAY_NO_DATA				ET_ERROR_BYTEARRAY(0x00000003)	/// No data in object
#define ET_ERROR_BYTEARRAY_INVALID_FORMAT		ET_ERROR_BYTEARRAY(0x00000004)	/// Data has invalid format
#define ET_ERROR_BYTEARRAY_MODIFY_END_OF_PARAMS	ET_ERROR_BYTEARRAY(0x00000005)	/// Data has invalid format
//------------------------------------------------------------------------
//	Protocol errors
//------------------------------------------------------------------------
#define ET_ERROR_PROTOCOL_MASK		0x00040000
#define ET_ERROR_PROTOCOL(error_code) ((error_code)|(ET_ERROR_PROTOCOL_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_PROTOCOL_FORMAT_ERROR				ET_ERROR_PROTOCOL(0x00000001)	/// Protocol command parse error in format
#define ET_ERROR_PROTOCOL_CRC_ERROR					ET_ERROR_PROTOCOL(0x00000002)	/// Protocol command parse error in CRC
#define ET_ERROR_PROTOCOL_CREATE_BYTE_ARRAY_FAILED	ET_ERROR_PROTOCOL(0x00000003)	/// Protocol command create array from command error
#define ET_ERROR_PROTOCOL_INVALID_SEQUENCE          ET_ERROR_PROTOCOL(0x00000004)   /// Protocol command recive invalid sequence
//------------------------------------------------------------------------
/// OS dependent functions errors
//------------------------------------------------------------------------
#define ET_ERROR_OS_DEPEND_MASK		0x00050000
#define ET_ERROR_OS_DEPEND(error_code) ((error_code)|(ET_ERROR_OS_DEPEND_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_OS_DEPEND_NOT_IMPLEMENTED	ET_ERROR_OS_DEPEND(0x00000001)	/// OS Dependent function not implemented
#define ET_ERROR_OS_DEPEND_FAILED			ET_ERROR_OS_DEPEND(0x00000002)	/// OS Dependent function call failed
//------------------------------------------------------------------------
/// Synchronization object errors
//------------------------------------------------------------------------
#define ET_ERROR_SYNC_OBJ_MASK		0x00060000
#define ET_ERROR_SYNC_OBJ(error_code) ((error_code)|(ET_ERROR_SYNC_OBJ_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_SYNC_OBJ_ABANDONED		ET_ERROR_SYNC_OBJ(0x00000002)		/// Synchronization object abandoned
#define ET_ERROR_SYNC_OBJ_CREATE		ET_ERROR_SYNC_OBJ(0x00000003)		/// Error creating synchronization object
//------------------------------------------------------------------------
/// Thread errors
//------------------------------------------------------------------------
#define ET_ERROR_THREAD_MASK		0x00080000
#define ET_ERROR_THREAD(error_code) ((error_code)|(ET_ERROR_THREAD_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_THREAD_ALREADY_RUNNING	ET_ERROR_THREAD(0x00000001)		/// Thread is already running
#define ET_ERROR_THREAD_NOT_RUNNING		ET_ERROR_THREAD(0x00000002)		/// Thread is not running
#define ET_ERROR_THREAD_WAS_NOT_RUN		ET_ERROR_THREAD(0x00000003)		/// Thread was not run
#define ET_ERROR_THREAD_START				ET_ERROR_THREAD(0x00000004)		/// Cannot start thread
#define ET_ERROR_THREAD_SUSPEND_FAILED	ET_ERROR_THREAD(0x00000005)		/// Cannot suspend thread
#define ET_ERROR_THREAD_RESUME_FAILED	ET_ERROR_THREAD(0x00000006)		/// Cannot resume thread
#define ET_ERROR_THREAD_ILLEGAL_CONTROL ET_ERROR_THREAD(0x00000007)		/// Cannot call start/wait from the same thread
//------------------------------------------------------------------------
/// Dll Library errors
//------------------------------------------------------------------------
#define ET_ERROR_LIBRARY_MASK		0x00090000
#define ET_ERROR_LIBRARY(error_code) ((error_code)|(ET_ERROR_LIBRARY_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_LIBRARY_SYSTEM_ERROR		ET_ERROR_LIBRARY(0x00000001)		/// Failed to load library
#define ET_ERROR_LIBRARY_FUNCTION_NOT_FOUND ET_ERROR_LIBRARY(0x00000002)		/// Function not found in library
#define ET_ERROR_LIBRARY_NOT_LOADED			ET_ERROR_LIBRARY(0x00000003)		/// Library not loaded

//------------------------------------------------------------------------
/// XML errors
//------------------------------------------------------------------------
#define ET_ERROR_XML_MASK		0x000A0000
#define ET_ERROR_XML(error_code) ((error_code)|(ET_ERROR_XML_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_XML_SYNTAX_ERROR		ET_ERROR_XML(0x00000001)		/// переданный файл содержит синтаксическую ошибку
#define ET_ERROR_XML_SYNTAX_EXCEPTION	 ET_ERROR_XML(0x00000002)		/// прочитанный тег является служебным, возможно продолжение работы

//------------------------------------------------------------------------
/// File I/O errors
//------------------------------------------------------------------------
#define ET_ERROR_FILE_MASK		0x000B0000
#define ET_ERROR_FILE(error_code) ((error_code)|(ET_ERROR_FILE_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_FILE_ERROR			ET_ERROR_FILE(0x00000001)		/// ошибка чтения/записи
#define ET_ERROR_FILE_NOT_FOUND		ET_ERROR_FILE(0x00000002)		/// файл не найден
#define ET_ERROR_FILE_EOF			ET_ERROR_FILE(0x00000003)		/// конец файла (End Of File)
//------------------------------------------------------------------------
/// Queue errors
//------------------------------------------------------------------------
#define ET_ERROR_QUEUE_MASK		0x000C0000
#define ET_ERROR_QUEUE(error_code) ((error_code)|(ET_ERROR_QUEUE_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_QUEUE_EMPTY			ET_ERROR_QUEUE(0x00000001)		/// очередь пустая
//------------------------------------------------------------------------
/// Connection errors
//------------------------------------------------------------------------
#define ET_ERROR_CONNECTION_MASK		0x000D0000
#define ET_ERROR_CONNECTION(error_code) ((error_code)|(ET_ERROR_CONNECTION_MASK))
//------------------------------------------------------------------------
#define ET_ERROR_CONNECTION_ANSWER_OBSOLETE	ET_ERROR_CONNECTION(0x00000001)		/// Answer to service command is obsolete
#define ET_ERROR_CONNECTION_NOT_CONNECTED	ET_ERROR_CONNECTION(0x00000002)		/// Not connected to server

//------------------------------------------------------------------------
/// User errors
//------------------------------------------------------------------------
#define ET_ERROR_USER_MASK		0x80000000
#define ET_ERROR_USER(error_id,error_code) ((error_code)|(error_id << 16)|(ET_ERROR_USER_MASK))
//------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// Error info
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#define ET_INFO_NO 0x00	/// No ErrorInfo
#define ET_INFO_BAD_SIZE    0x01
#define ET_INFO_BAD_DATA    0x02
#define ET_INFO_CANCELED    0xFF    

// Информация об ошибках, связанная с соединением с сервером
// во время ошибки при связи с сервером возникла неопределенность со статусом оплаты
#define ET_INFO_RESULT_UNPREDICTED 0x01

//------------------------------------------------------------------------
/// ET_ERROR_NULL_POINTER
//------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
/// Error info Card Reader
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#define ET_ERROR_PORT_CARD_READER_MASK       			0x01000000
#define ET_ERROR_CARD_READER(error_code) ((error_code)|(ET_ERROR_PORT_CARD_READER_MASK))
#define ET_ERROR_CARD_READER_         ET_ERROR_CARD_READER(0x00000001)	/// Port Error


*/



#endif
