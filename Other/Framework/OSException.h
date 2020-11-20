// Uniteller.Framework
// Version: 1.0.0
// Generation: 1

//  ���� �������� ����� ���������� (OSException), ������� ���������� ������������ ��������� � WinAPI.
//
//  void testUsing()
//  {
//    m_keepSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
//    if (NULL == m_keepSignal) {
//	    throw OSException("CreateEvent: ");
//    }
//  }


#pragma once

#include <exception>
#include <windows.h>


/*
#include <system_error>
OSException(std::string& prevMessage = std::string())
{
    DWORD dwErrVal = GetLastError();
    return system_error((dwErrVal, std::system_category()),prevMessage)
    return std::system_error(dwErrVal, std::system_category(), prevMessage);
}
*/


//namespace Uniteller
//{
//  namespace Framework
//  { 
    
    /// <summary>
    /// ����� ����������, ������� ���������� ������������ ��������� � WinAPI.
    /// �������� ����������� std::exception
    /// </summary>
    class OSException	: public std::exception
    {
      public:

        /// <summary>
        /// ����������� ���������� ������� ����� ��������� � ���� �������� � ��� ������ WinAPI �������.
        /// ����������������� ����� �������� ����������� std::exception � ��������� ����� what() ������� ����� ����:
        ///   "���������� prevMessage"+"�������� ��������� ������ ���������� ������� ������� GetLastError()"
        /// </summary>
        /// <param name="prevMessage">�������� ��������� ��� ��������� ������, ���� �����</param>
        OSException(char* prevMessage = NULL)
            : m_Error(GetLastError())
        {
			*(static_cast<std::exception*>(this)) = std::exception( systemErrorMessage(m_Error, std::string(NULL == prevMessage ? "" : prevMessage)).c_str() );
        }

        /// <summary>
        /// ���������� ��� ��������� (WinAPI) ������, ���������� ������� ������� GetLastError() � ������������ ������
        /// </summary>
        /// <returns>��� ������</returns>
        DWORD SysError()
        {
            return m_Error;
        }        

    private:

        DWORD m_Error;    // ��� ��������� (WinAPI) ������

        /// <summary>
        /// ��������� ����� ���������, ������� ����� ������������ ������� std::exception.what()
        /// �������� ����� ����� ���:
        ///   "���������� prevMessage"+"�������� ��������� ������ ���������� ������� ������� GetLastError()"
        /// </summary>
        /// <param name="prevMessage">��� ��������� (WinAPI) ������</param>
        /// <param name="prevMessage">�������� ��������� ��� ��������� ������, ���� �����</param>
        static std::string& systemErrorMessage(DWORD _error, std::string& prevMessage = std::string())
        {
            if (_error == 0)
		        return prevMessage;

            size_t oldLen = prevMessage.length();
            prevMessage.resize(oldLen + 1024);
	        const size_t length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, _error, 0, &prevMessage[oldLen], prevMessage.length() - oldLen, 0);
            prevMessage.resize(length+oldLen);

            return prevMessage;
        }
  };
//  } // ������������ ���� Framework 
//} // ������������ ���� Uniteller