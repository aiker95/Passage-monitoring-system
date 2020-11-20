#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sstream>
#include <string>
#include "ErrorType.h"

namespace UFramework {
	
	// Levels:
	// 0 - Debug (Default);
	// 1 - Error;
	// 2 - Info;
	// Others - Debug.
	typedef void (*tLogWriter) (const std::string &rec, int level);

	class EndLog{};

	class CLogger
	{
	public:

		static void SetLogWriter(tLogWriter _logWriter)
		{
			g_logWriter = _logWriter;
		}

		/// Конструктор
		CLogger(const char* pref, const char* locateInFilename, unsigned long locateInLine )
			: m_locateInFilename(locateInFilename)
			, m_locateInLine(locateInLine)
		{
			m_logStream << pref;
		}

		CLogger& operator<< (const EndLog &val )
		{
			if ( g_logWriter )
			{
				m_logStream << " // Ln " << m_locateInLine << " in " << m_locateInFilename;
				g_logWriter(m_logStream.str(), 0);
			}
			this->clear();

			return *this;
		}

		CLogger& operator<<(const CF_ErrorType &aValue)
		{
			m_logStream << aValue.m_dwErrorCode << ':' << aValue.m_dwErrorInfo;

			return *this;
		}
		
		template<class T>
		CLogger& operator<<(const T* aValue)
		{
			if (aValue)
				m_logStream << aValue;

			return *this;
		}
		
		template<class T>
		CLogger& operator<<(T* aValue)
		{
			if (aValue)
				m_logStream << aValue;

			return *this;
		}
		
		template<class T>
		CLogger& operator<<(const T& aValue)
		{
			m_logStream << aValue;

			return *this;
		}

		void HexOn()
		{	
			m_logStream << std::hex;
		}

		CLogger& HexToString(const unsigned char * buf, size_t length)
		{	
			m_logStream << std::hex;
			for (size_t i = 0; i < length; ++i)
			{
				if (i > 0 && 0 == (i & 0x0f)) {
					 //m_logStream << "\r\n";
					if (g_logWriter) g_logWriter(m_logStream.str(), 0);
					 this->clear();
				}

				m_logStream.fill('0');
				m_logStream.width(2);
				m_logStream << (unsigned int )buf[i] << ' ';
			}
			return *this;
		}

		void DecOn()
		{
			m_logStream << std::dec;
		}

		void clear()
		{
			m_logStream.flush();
			//m_logStream.clear();
			m_logStream.str("");
		}

	protected:

		/// Конструктор
		CLogger(){}

		static tLogWriter		g_logWriter;
		std::stringstream		m_logStream;
		std::string				m_locateInFilename;
		unsigned long			m_locateInLine;
	};
}



//#define LOG_ADD_WRITER( logLevel, writerType, configurationString ) COMENT_NEXT
//#define LOG_FN_START	CLogger( ">> "__FUNCTION__, __FILE__, __LINE__  )
//#define LOG_FN_STOP   CLogger( "<< "__FUNCTION__, __FILE__, __LINE__  )
#define LOG_TRACE		UFramework::CLogger( "!SLK! - ", __FILE__, __LINE__  )
#define LOG_WORNING		UFramework::CLogger( "!SLK! Wr: ", __FILE__, __LINE__  )
#define LOG_ERROR		UFramework::CLogger( "!SLK! Erorr: ", __FILE__, __LINE__  )

//#define LOG_FN_START_F( format, ...) COMENT_NEXT
//#define LOG_TRACE_F( format, ...)	COMENT_NEXT

#define END UFramework::EndLog()


//////////////////////////////////////////////////////////////////////////

#endif //#ifndef _LOGGER_H_
