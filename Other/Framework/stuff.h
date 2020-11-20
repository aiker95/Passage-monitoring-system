#ifndef _STUFF_H_
#define _STUFF_H_

#include <string>
#include <map>
#include <algorithm>

#if defined(_WIN32_WINNT) && (WINVER >= _WIN32_WINNT)
#include <atlbase.h>
#include <atlsafe.h>
#define ATL_INCLUDED

#define FILE_VERSION_INCLUDED
#endif

#include "ErrorType.h"


/// Wrap literal in quotes for either ANSI or UNICODE
#define     ASTRINGIFY(x)    #x
#define     WSTRINGIFY(x)    L#x
/// Converts digital literal to string in either ANSI or UNICODE
#define     TO_STRING(x)      ASTRINGIFY(x)
#define     TO_STRING_U(x)    WSTRINGIFY(x)


namespace stuff
{
	using namespace std;
    
	typedef map<string, string> StringMap;
	
	struct SerializeSettings {
			string ignoreSymbols;
			char parDivider;
			char keyValDivider;
			char startFraming;
			char endFraming;
	};
	extern SerializeSettings defaultSerlzSetings; // = { " \t\r\n", ':', ',', '<', '>' };

	///////////////////////////////////////////////////////////////
	/// @brief WToC - Converts Unicode string to ASCII.
	/// 
	/// @param wstr - pointer to null-terminated string
	///
	/// @return char* - pointer to allocated null-terminated string
	///////////////////////////////////////////////////////////////
	//char* WToC(const wchar_t* wstr);
	std::string WcharToChar(const std::wstring &wstr);

	///////////////////////////////////////////////////////////////
	/// @brief CToW - Converts ASCII string to Unicode
	/// 
	/// @param str - pointer to null-terminated string
	///
	/// @return wchar_t* - pointer to allocated null-terminated string
	///////////////////////////////////////////////////////////////
	//wchar_t* CToW(const char* str);
	std::wstring CharToWchar(const std::string &str);

	char* WToUTF8(const wchar_t* wstr);

	//////////////////////////////////////////////////////////////////////////
	///  Converts byte array to std::map<std::string, std::string>
	//////////////////////////////////////////////////////////////////////////
	size_t UnserializeStringMap( StringMap &ouputMap, char * input, size_t inputSize, SerializeSettings setng);

	//////////////////////////////////////////////////////////////////////////
	/// Converts std::map<std::string, std::string> to byte array
	//////////////////////////////////////////////////////////////////////////
	void SerializeStringMap( const StringMap &input, unsigned char ** output, unsigned int *outputSize);

#ifdef ATL_INCLUDED
	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts std::string to BSTR
	///
	/// @param[in] input - std::string
	///
	/// @return CComBSTR string class
	//////////////////////////////////////////////////////////////////////////
	CComBSTR StrToBSTR( std::string input);

	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts BSTR to std::string
	///
	/// @param[in] input - BSTR string
	///
	/// @return std::string class
	//////////////////////////////////////////////////////////////////////////
	std::string BSTRToStr( CComBSTR input);

	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts std::map<std::string,std::string> to 2D Safe Array of BSTR
	///
	/// @param[in] input - std::map<std::string,std::string> array
	///
	/// @return CComSafeArray class
	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<BSTR> StrMapToSafeArray( std::map<std::string,std::string> input );

	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts 2D Safe Array of BSTR to std::map<std::string,std::string>
	///
	/// @param[in] input - CComSafeArray array
	///
	/// @return std::map<std::string,std::string> class
	//////////////////////////////////////////////////////////////////////////
	std::map<std::string,std::string> SafeArrayToStrMap( CComSafeArray<BSTR> input );
#endif

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string makeUpper(std::string text);
	std::string makeLower(std::string text);


	//////////////////////////////////////////////////////////////////////////
	/// @brief Checks filePath. Just returns if path is absolute or returns combined path if relative 
	//////////////////////////////////////////////////////////////////////////
	std::string CheckPath( std::string PathToCheck, std::string SourcePath );
	//////////////////////////////////////////////////////////////////////////
	/// @brief Checks if path exists.
	//////////////////////////////////////////////////////////////////////////
	bool FileExists( std::string FilePath );
	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets current process filename
	//////////////////////////////////////////////////////////////////////////
	std::string GetProcessFilename();
	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets current module filename
	//////////////////////////////////////////////////////////////////////////
	std::string GetModuleFilename();
	//////////////////////////////////////////////////////////////////////////
	/// @brief Get path without file
	//////////////////////////////////////////////////////////////////////////
	std::string GetPathWithoutFileSpec(std::string FullPath);
	//////////////////////////////////////////////////////////////////////////
	/// @brief IsDirectory
	//////////////////////////////////////////////////////////////////////////
	bool IsDirectory(std::string FilePath );
	//////////////////////////////////////////////////////////////////////////
	/// @biref Creates compound path from two parts
	//////////////////////////////////////////////////////////////////////////
	std::string CombinePath( std::string DirPath, std::string FilePath );
	//////////////////////////////////////////////////////////////////////////
	/// @brief Check if file path is full
	//////////////////////////////////////////////////////////////////////////
	bool PathIsFull( std::string FilePath );


	//std::wstring FindFileName( std::wstring &name );

#ifdef FILE_VERSION_INCLUDED
	struct FileVersion {
		WORD VersionWord[4];
	};

	//////////////////////////////////////////////////////////////////////////
	/// @brief Returns version of dll or exe
	//////////////////////////////////////////////////////////////////////////
	FileVersion GetFileVersion( std::string FilePath );
#endif

	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets buffer length for conversion from UNICODE to UTF-8. Without ending 0.
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetUTF8BufferLength( const wchar_t *inputString );
	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets converted string from UNICODE to UTF-8
	//////////////////////////////////////////////////////////////////////////
	char *GetUFT8Buffer( const wchar_t *inputString );
	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts hex error string to DWORD
	//////////////////////////////////////////////////////////////////////////
	DWORD GetHexErrorCode( const char *errorHex );

	//UFramework::CF_ErrorType ByteToChars(UCHAR byte, char *Hi, char *Lo);
	void ByteToChars(UCHAR byte, char &Hi, char &Lo);
	void ByteToChars(UCHAR byte, wchar_t &Hi, wchar_t &Lo);
	unsigned char HexToBin(char hex);

	std::string ArrayToString(unsigned char *array, unsigned int arraySize);
	void StringToArray(std::string stringArray, unsigned char ** array, unsigned int *arraySize);
	UFramework::CF_ErrorType CharsToByte( char Hi, char Lo, UCHAR *byte);
	char *ArrayToHex(const BYTE *data, unsigned int dataSize);
	char *ArrayToHex(const BYTE *data, unsigned int dataSize, char *hexStr, unsigned int hexStrLen);
	UFramework::CF_ErrorType HexToByte(const char *hexStr, UCHAR *buffer, UINT *bufferSize);
	UFramework::CF_ErrorType ByteToHex(UCHAR *bytes, UINT bytesCount, char *hexStr, UINT *hexStrSize);

	//	std::string CFStrToStdStr(UFramework::CF_String &str);

	bool HexToArray(const char *str, unsigned char *hexData, int size);
	unsigned char* HexToArray(const char *str, int &arraySize);
	unsigned char* HexToArray(const char *str);

	bool IsHexString(const char* str);
	bool IsHexString(std::string str);

	bool IsDigitString(const char* str);	
	bool IsDigitString(std::string str);

	std::string&  IntToDigitstring(int val, std::string  &res = std::string() );
	std::wstring& IntToDigitstring(int val, std::wstring &res = std::wstring() );

    std::string timeToISO8601(SYSTEMTIME& tm);
}

#endif