// Uniteller.Framework
// Version: 1.0.0
// Generation: 1

//  Файд содержит класс исключения (OSException), который необходимо использовать совместно с WinAPI.
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
    /// Класс исключения, который необходимо использовать совместно с WinAPI.
    /// Является наследником std::exception
    /// </summary>
    class OSException	: public std::exception
    {
      public:

        /// <summary>
        /// Конструктор исключения которое будет содержать в себе описание и код ошибки WinAPI функции.
        /// Сконструированный класс является наследником std::exception и используя метод what() получим фразу вида:
        ///   "Содержимое prevMessage"+"Описание системной ошибки полученной вызовом функции GetLastError()"
        /// </summary>
        /// <param name="prevMessage">Описание контекста где произошла ошибка, если нужно</param>
        OSException(char* prevMessage = NULL)
            : m_Error(GetLastError())
        {
			*(static_cast<std::exception*>(this)) = std::exception( systemErrorMessage(m_Error, std::string(NULL == prevMessage ? "" : prevMessage)).c_str() );
        }

        /// <summary>
        /// Возвращает код системной (WinAPI) ошибки, полученной вызовом функции GetLastError() в конструкторе класса
        /// </summary>
        /// <returns>код ошибки</returns>
        DWORD SysError()
        {
            return m_Error;
        }        

    private:

        DWORD m_Error;    // Код системной (WinAPI) ошибки

        /// <summary>
        /// Формирует текст сообщения, который будет возвращаться методом std::exception.what()
        /// Итоговый текст имеет вид:
        ///   "Содержимое prevMessage"+"Описание системной ошибки полученной вызовом функции GetLastError()"
        /// </summary>
        /// <param name="prevMessage">Код системной (WinAPI) ошибки</param>
        /// <param name="prevMessage">Описание контекста где произошла ошибка, если нужно</param>
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
//  } // пространство имен Framework 
//} // пространство имен Uniteller