//////////////////////////////////////////////////////////////////////////
/// @file CF_ErrorType.h
/// @brief Contains declarations and definitions of CF_ErrorType structure
//////////////////////////////////////////////////////////////////////////

#ifndef _ERRORTYPE_H_
#define _ERRORTYPE_H_

#include "common_includes.h"
#include "ErrorDefines.h"

//////////////////////////////////////////////////////////////////////////
#define 	ERR_CreateInstance( errorCode , errorInfo ) CF_ErrorType( (errorCode), (errorInfo) )

/// Makes CF_ErrorType object when ewerything is ok
#define ErrorOK CF_ErrorType( 0, 0 )
#define ET_MakeError( errorCode , errorInfo ) CF_ErrorType( (errorCode), (errorInfo) )

namespace UFramework {

	struct CF_ErrorType {
	  
		unsigned long	m_dwErrorCode;	/// Stores Error Code
		unsigned long	m_dwErrorInfo;	/// Stores Error Info
		
	public:
		CF_ErrorType () /// Default constructor
			: m_dwErrorCode(0)
			, m_dwErrorInfo(0)
		{
		}
		
		CF_ErrorType ( unsigned long errorCode, unsigned long errorInfo) /// Initializing constructor
			: m_dwErrorCode(errorCode)
			, m_dwErrorInfo(errorInfo)
		{
		}
		
		bool operator == (CF_ErrorType operand) /// Compares ErrorCode and ErrorInfo of two objects
		{
			if ((m_dwErrorCode == operand.m_dwErrorCode) &&
				(m_dwErrorInfo == operand.m_dwErrorInfo)) {
					return true;
			}
			
			return false;
		}
		
		bool operator != (CF_ErrorType operand) /// Compares ErrorCode and ErrorInfo of two objects
		{
			if ((m_dwErrorCode != operand.m_dwErrorCode) ||
				(m_dwErrorInfo != operand.m_dwErrorInfo)) {
					return true;
			}
			
			return false;
		}

		bool IsOk() /// Check if there is an error
		{
			if ( m_dwErrorCode || m_dwErrorInfo )
					return false;
			
			return true;
		}

		unsigned long GetErrorCode() const	/// Returns Error code
		{
			return m_dwErrorCode;
		}
		
		unsigned long GetErrorInfo() const	/// Returns Error info
		{
			return m_dwErrorInfo;
		}
		
	};

}

#endif