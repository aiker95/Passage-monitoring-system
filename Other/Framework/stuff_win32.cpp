#include "stdafx.h"

#undef AUTO_GENERATE_ERROR_TO_LOG


#include "stuff.h"
#include "common_includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>



#ifdef FILE_VERSION_INCLUDED
#include <Psapi.h>
#pragma comment( lib, "Psapi" )
#pragma comment( lib, "Version")
//Psapi.lib Version.lib
#endif

//////////////////////////////////////////////////////////////////////////

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

//////////////////////////////////////////////////////////////////////////

using namespace UFramework;
namespace stuff
{
	//////////////////////////////////////////////////////////////////////////
	/// WToC - Converts Unicode string to ASCII.
	//////////////////////////////////////////////////////////////////////////
	/*char* WToC(const wchar_t* wstr)
	{
		char* str;
		if(wstr==NULL)
			str=_strdup("");
		else if(wstr[0]=='\0')
			str=_strdup("");
		else
		{
			int len = WideCharToMultiByte(UF_DEF_ENCODING,0,wstr,-1,NULL,0,NULL,NULL);
			if(len==0)
			{
				len = WideCharToMultiByte(CP_ACP,0,wstr,-1,NULL,0,NULL,NULL);
				if(len==0)
					str=_strdup("");
				else
				{
					str = (char*)malloc(len+2);
					WideCharToMultiByte(CP_ACP,0,wstr,-1,str,len+1,NULL,NULL);
				}		
			}
			else
			{
				str = (char*)malloc(len+2);
				WideCharToMultiByte(UF_DEF_ENCODING,0,wstr,-1,str,len+1,NULL,NULL);
			}

		}

		return str;
	}*/

	std::string timeToISO8601(SYSTEMTIME& tm)
	{
		char		strTimeMark[32] = "\0";

		_TIME_ZONE_INFORMATION  utcZone;
		//SYSTEMTIME				tm;
		GetTimeZoneInformation(&utcZone);
		//GetLocalTime(&tm);

		int hShift = abs(utcZone.Bias) / 60;
		int mShift = abs(utcZone.Bias) % 60;
		int n = _snprintf(strTimeMark, sizeof(strTimeMark), "%02d-%02d-%dT%02d:%02d:%02d+%02d:%02d",
			tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, hShift, mShift);
		return strTimeMark;
	}


	std::string WcharToChar(const std::wstring &wstr)
	{
		std::string str; 

		UINT     codePage = UF_DEF_ENCODING;
		int len = WideCharToMultiByte(codePage, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
		if( 0 == len)
		{
			codePage = CP_ACP;
			len = WideCharToMultiByte(codePage, 0, wstr.c_str(), wstr.length(), NULL, 0, ".", NULL);
		}

		if( 0 == len)
			return str;	

		str.resize(len+1);
		len = WideCharToMultiByte(codePage, 0, wstr.c_str(), wstr.length(), &str[0], len+1, ".", NULL);
		str.resize(len);

		return str;
	}

	//////////////////////////////////////////////////////////////////////////
	/// CToW - Converts ASCII string to Unicode
	//////////////////////////////////////////////////////////////////////////
	/*wchar_t* CToW(const char* str)
	{
		wchar_t* wstr;
		if(str==NULL)
			wstr=_wcsdup(L"");
		else if(str[0]=='\0')
			wstr=_wcsdup(L"");
		else
		{
			int len = MultiByteToWideChar(UF_DEF_ENCODING,0,str,-1,NULL,0);
			if(len==0)
			{
				len = MultiByteToWideChar(CP_ACP,0,str,-1,NULL,0);
				if(len==0)
					wstr=_wcsdup(L"");
				else
				{
					wstr = (wchar_t*)malloc( (len+1)*sizeof(wchar_t) );
					MultiByteToWideChar(CP_ACP,0,str,-1,wstr,len+1);
				}
			}
			else
			{
				wstr = (wchar_t*)malloc( (len+1)*sizeof(wchar_t) );
				MultiByteToWideChar(UF_DEF_ENCODING,0,str,-1,wstr,len+1);
			}
		}

		return wstr;
	}*/
	std::wstring CharToWchar(const std::string &str)
	{
		std::wstring  wstr;

		UINT     codePage = UF_DEF_ENCODING;
		int len = MultiByteToWideChar(codePage, 0, str.c_str(), str.length(), NULL, 0);
		if(len == 0)
		{
			codePage = CP_ACP;
			len = MultiByteToWideChar(codePage, 0, str.c_str(), str.length(), NULL, 0);

		}

		if(len == 0)
			return wstr;

		wstr.resize(len+1);
		len = MultiByteToWideChar(codePage, 0, str.c_str(), str.length(), &wstr[0], len+1);
		wstr.resize(len);

		return wstr;
	}


	char* WToUTF8( const wchar_t* wstr )
	{
		char* str;
		int len = WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);
		str = (char*)malloc(len+2);
		WideCharToMultiByte(CP_UTF8,0,wstr,-1,str,len+1,NULL,NULL);

		return str;
	}

	//std::string CFStrToStdStr(CF_String &str)
	//{
	//	std::string res = "";
	//	char* buf = str.CopyBufferA();
	//	if(buf!=NULL)
	//		res = buf;
	//	delete [] buf;
	//	return res;
	//}


	//////////////////////////////////////////////////////////////////////////
	/// Converts std::map<std::string, std::string> to byte array
	//////////////////////////////////////////////////////////////////////////
	void SerializeStringMap( const StringMap &input, unsigned char ** output, unsigned int *outputSize)
	{
		StringMap::const_iterator  it;

		if ((outputSize == NULL) || (output == NULL)) {
			return;
		}

		*outputSize = 0;

		*output = NULL;
		unsigned int dataSize = 0;

		for (it =  input.begin(); it != input.end(); it++) {

			dataSize += (it->first.length()+1)+(it->second.length()+1)+1;
		}

		if (dataSize == 0) {
			*outputSize = 0;
			*output = NULL;
		} else {

			unsigned char * outputArray = new unsigned char[ dataSize ];
			//*output = new BYTE[ dataSize ];
			if (outputArray != NULL) {
				memset(outputArray, 0x00, dataSize);

				unsigned int position = 0;
				for (it =  input.begin(); it != input.end(); it++) {
					memcpy(outputArray+position,it->first.c_str(),it->first.length());
					position += it->first.length()+1;
					memcpy(outputArray+position,it->second.c_str(),it->second.length());
					position += it->second.length()+1;
					position ++;
				}

				*output = outputArray;
				*outputSize = dataSize;

			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	///  Converts byte array to std::map<std::string, std::string>
	//////////////////////////////////////////////////////////////////////////

SerializeSettings defaultSerlzSetings = { " \t\r\n", ',', ':', '<', '>' };
	size_t UnserializeStringMap( StringMap &ouputMap, char * input, size_t inputSize, SerializeSettings setng = defaultSerlzSetings)
	{

		if ((input == NULL) || (inputSize == 0)) return 0;

		char*  endOf = input + inputSize;
		char*  curr  = input;
		string	key, val;

		while (curr < endOf)
		{
			key.erase();
			val.erase();

			// ignore state
			for (; curr < endOf; ++curr)
				if ( string::npos == setng.ignoreSymbols.find(*curr) ) 
					break;

			if (curr >= endOf) break;
			
			// chek framming state
			if (setng.startFraming != 0) {
				if (setng.startFraming != *curr)
				   break;
				else
					++curr;
			}

			// collecting key state (befor framming not arrived)
			char endFraming =(setng.endFraming != 0) ? setng.endFraming : setng.keyValDivider;
			for (; curr < endOf; ++curr)
				if (endFraming == *curr) 
					break;
				else 
					key += *curr;

			if (curr >= endOf) break;

			// chek key-val divider state
			if (endFraming != setng.keyValDivider) ++curr;
			if (setng.keyValDivider != *curr)
				break;
			else
				++curr;

			if (curr >= endOf) break;			
			// chek framming state
			if (setng.startFraming != 0) {
				if (setng.startFraming != *curr)
					endFraming = setng.parDivider; // For JASON, becous it don't use frame for integer value
				else {
					++curr;
					endFraming =(setng.endFraming != 0) ? setng.endFraming : setng.parDivider;
				}
			}
			else 
				endFraming = setng.parDivider;
			
			// collecting val state (befor framming not arrived)
			for (; curr < endOf; ++curr)
				if (endFraming == *curr) 
					break;
				else 
					val += *curr;
			
			if (curr >= endOf) break;

			ouputMap[key] = val;

			// chek key-val divider state
			if (endFraming != setng.parDivider) ++curr;
			if (setng.parDivider != *curr)
				break;
			else
				++curr;
		}

		return ouputMap.size(); 
	}


#ifdef ATL_INCLUDED
	//////////////////////////////////////////////////////////////////////////
	/// Converts std::string to BSTR	
	//////////////////////////////////////////////////////////////////////////
	CComBSTR StrToBSTR( std::string input ) {	
		CComBSTR result( input.c_str() );
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	/// Converts BSTR to std::string
	//////////////////////////////////////////////////////////////////////////
	std::string BSTRToStr( CComBSTR input) {
/*
		char *tempString = WToC((wchar_t*)input);
		std::string result = tempString;
		free(tempString);
		return result;*/
		return (WcharToChar((wchar_t*)input));
	}

	//////////////////////////////////////////////////////////////////////////
	/// Converts std::map<std::string,std::string> to 2D Safe Array of BSTR	
	//////////////////////////////////////////////////////////////////////////
	CComSafeArray<BSTR> StrMapToSafeArray( std::map<std::string,std::string> input ) {

		unsigned int dataCount = (unsigned int)input.size();

		CComSafeArrayBound bound[2];
		bound[0].SetCount(2);
		bound[0].SetLowerBound(0);
		bound[1].SetCount(dataCount);
		bound[1].SetLowerBound(0);

		CComSafeArray<BSTR> resultArray(bound,2);

		std::map<std::string,std::string>::iterator it;

		LONG aIndex[2];

		unsigned int i=0;
		for( it = input.begin(); it != input.end(); it++ ) {

			/// Set parameter name;
			aIndex[0] = 0;
			aIndex[1] = i;

			HRESULT hr = resultArray.MultiDimSetAt(aIndex,StrToBSTR(it->first));
			if (SUCCEEDED(hr)) {

				/// Set paramete value;
				aIndex[0] = 1;
				aIndex[1] = i;

				resultArray.MultiDimSetAt(aIndex,StrToBSTR(it->second));
			}

			i++;
		}

		return resultArray;
	}

	//////////////////////////////////////////////////////////////////////////
	/// Converts 2D Safe Array of BSTR to std::map<std::string,std::string>	
	//////////////////////////////////////////////////////////////////////////
	std::map<std::string,std::string> SafeArrayToStrMap( CComSafeArray<BSTR> input ) {

		bool error = false;

		std::map<std::string,std::string> result;
		UINT Dimensions = input.GetDimensions();

		if (Dimensions != 2) error = true;

		LONG firstDimensionCount = 0;
		LONG firstDimensionLowBound = 0;
		LONG ParametersCount = 0;
		LONG ParametersStartIndex = 0;

		/// Check array bounds
		if (!error) {
			firstDimensionCount = input.GetCount(0);
			firstDimensionLowBound = input.GetLowerBound(0);
			ParametersCount = input.GetCount(1);
			ParametersStartIndex = input.GetLowerBound(1);

			if ( (firstDimensionCount != 2) || 
				(firstDimensionLowBound != 0) ||
				(ParametersCount == 0) )
			{

				error = true;			
			}
		}

		/// Convert array
		if (!error) {

			LONG aIndex[2] = {0, 0};
			std::string parameterName;
			std::string parameterValue;
			for (LONG index = ParametersStartIndex; index < ParametersStartIndex+ParametersCount; index++)
			{
				/// Set parameter name;
				aIndex[0] = 0;
				aIndex[1] = index;

				BSTR temp;
				HRESULT hr = input.MultiDimGetAt(aIndex,temp);
				if (SUCCEEDED(hr)) {
					parameterName = BSTRToStr( temp );
					SysFreeString( temp );

					/// Set parameter value;
					aIndex[0] = 1;
					aIndex[1] = index;

					hr = input.MultiDimGetAt(aIndex,temp);
					if (SUCCEEDED(hr)) {
						parameterValue = BSTRToStr( temp );
						SysFreeString( temp );

						result[parameterName] = parameterValue;
					}
				}				
			}
		}

		return result;
	}
#endif


	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Makes text uppercase
	std::string makeUpper(std::string text)
	{
		std::string temp = text;
		std::transform( temp.begin(), temp.end(), temp.begin(), toupper);
		return temp;
	}

	//! Makes text lowercase
	std::string makeLower(std::string text)
	{
		std::string temp = text;
		std::transform( temp.begin(), temp.end(), temp.begin(), tolower);
		return temp;
	}


	/////////////////////////////////
	/// Converts char to byte "0" -> 0x00 ... "F" -> "0x0f"
	/////////////////////////////////
	CF_ErrorType CharsToByte( char Hi,  char Lo, UCHAR *byte) {
		/// Checks input data
		if (byte == NULL) return ERR_CreateInstance( ERR_NULL_POINTER, 0);

		UCHAR localByte = 0x00;

		if ((Hi >= '0') && (Hi <= '9')) {
			localByte = Hi - '0';
		} else if ((Hi >= 'A') && (Hi <= 'F')) {
			localByte = Hi - 'A'+0x0a;
		} else if ((Hi >= 'a') && (Hi <= 'f')) {
			localByte = Hi - 'a'+0x0a;
		} else {
			return ERR_CreateInstance( ERR_INVALID_ARGUMENT, 0);
		}

		localByte = localByte << 4;

		if ((Lo >= '0') && (Lo <= '9')) {
			localByte += Lo - '0';
		} else if ((Lo >= 'A') && (Lo <= 'F')) {
			localByte += Lo - 'A'+0x0a;
		} else if ((Lo >= 'a') && (Lo <= 'f')) {
			localByte += Lo - 'a'+0x0a;
		} else {
			return ERR_CreateInstance( ERR_INVALID_ARGUMENT, 0);
		}
		*byte = localByte;

		return ErrorOK;
	}
	/////////////////////////////////
	/// Converts byte to char 0x00 -> "00" 0xff -> "FF"
	/////////////////////////////////
	void ByteToChars(UCHAR byte, char &Hi, char &Lo)
	{
		static const char hexSymb[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		Hi = hexSymb[byte >> 4];
		Lo = hexSymb[byte & 0x0F];
	}

	void ByteToChars(UCHAR byte, wchar_t &Hi, wchar_t &Lo)
	{
		static const wchar_t hexSymb[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		Hi = hexSymb[byte >> 4];
		Lo = hexSymb[byte & 0x0F];
	}

	inline unsigned char HexToBin(char hex)
	{
		if ( (hex >= '0') && (hex <= '9') )
			return hex - '0';
		else if ( (hex >= 'A') && (hex <= 'F') )
			return hex - 'A'+0x0a;
		else if ( (hex >= 'a') && (hex <= 'f') )
			return hex - 'a'+0x0a;
		else
			return 0xff;
	}

	bool IsHexString(const char* str)
	{
		return IsHexString((std::string)str);
	}

	bool IsHexString(std::string str)
	{
		int strSize = str.length();
		int arrayLen = strSize >> 1;

		if ( (strSize & 0x01) != 0 ) // check even %1
			return false;

		for(int i=0; i < strSize; i++)
		{
			if( HexToBin( str[i] ) > 0xf )
				return false;
		}

		return true;
	}



	bool IsDigitString(const char* str)
	{
		return IsDigitString((std::string)str);
	}

	bool IsDigitString(std::string str)
	{
		for each(const char &c in str)
		{
			if ( (c < '0') || (c > '9') )
				return false;
		}

		return true;
	}


	bool HexToArray(const char *str, unsigned char *hexData, int size) 
	{
		if (str == NULL)
			return false;

		for (int i = 0; i < size; ++i)
		{      
			unsigned char bh = HexToBin(str[i << 1]);
			if( bh > 0xf )
				return false;

			unsigned char bl = HexToBin(str[(i << 1) + 1]);
			if( bl > 0xf )
				return false;

			hexData[i] = (bh << 4) | bl;
		}

		return true;
	}

	/////////////////////////////////
	/// Returns pointer to array of bytes;
	/////////////////////////////////
	unsigned char* HexToArray(const char *str, int &arraySize) 
	{
		if (str == NULL)
			return NULL;

		unsigned int strSize = strlen( str );

		if ( (strSize & 0x01) != 0 ) // check even
			return NULL;

		arraySize = strSize >> 1;
		unsigned char* hex = new unsigned char[arraySize];

		if ( !HexToArray(str, hex, arraySize) )
		{
			arraySize = 0;
			delete[] hex;
			hex = NULL;
		}

		return hex;
	}

	unsigned char* HexToArray(const char *str)
	{

		int arraySize;
		return HexToArray(str, arraySize);

		//return new unsigned char[8];
	}

	/////////////////////////////////
	/// Converts String to bytes array /// "02894ABC" -> {0x02, 0x89, 0x4A, 0XBC }
	/////////////////////////////////
	CF_ErrorType HexToByte(const char *hexStr, UCHAR *buffer, UINT *bufferSize) {
		/// Check input data
		if (hexStr == NULL) {
			return ERR_CreateInstance(ERR_NULL_POINTER, 0);
		}
		if (bufferSize == NULL) {
			return ERR_CreateInstance(ERR_NULL_POINTER, 0);
		}
		UINT hexStrLength = strlen(hexStr);
		/// Reject odd number of chars
		if (hexStrLength % 2 == 1) {
			return ERR_CreateInstance(ERR_INVALID_ARGUMENT,0);
		}
		/// No buffer ptr
		if (buffer == NULL) {
			*bufferSize = hexStrLength / 2;
			return ErrorOK;
		}
		/// Buffer != NULL
		/// Checks bufferSize
		if (*bufferSize < hexStrLength/2) {
			return ERR_CreateInstance(ERR_INVALID_ARGUMENT, 0);
		}

		UINT localBufferSize = hexStrLength / 2;
		UCHAR *localBuffer = new UCHAR[localBufferSize];
		ZeroMemory(localBuffer,localBufferSize);

		CF_ErrorType error = ErrorOK;
		/// Converting data
		for (UINT byte = 0; byte < hexStrLength / 2 ; byte++) {
			error = CharsToByte(hexStr[byte*2],hexStr[byte*2+1],localBuffer+byte);
			if (!error.IsOk()) {	break; }
		}

		if (error.IsOk()) {
			memcpy(buffer,localBuffer, localBufferSize);
			*bufferSize = hexStrLength / 2;
		}

		delete [] localBuffer;

		return error;
	}


	/////////////////////////////////
	/// Returns pointer to null-term string
	/////////////////////////////////
	char *ArrayToHex(const BYTE *data, unsigned int dataSize)
	{
		unsigned int hexLength = dataSize*2;
		char *hexStr = new char[hexLength+1];

		for (unsigned int i=0; i < dataSize; i++) {
			ByteToChars(*(data+i),hexStr[i*2],hexStr[i*2+1]);
		}

		hexStr[hexLength] = 0;

		return hexStr;    
	}

	/////////////////////////////////
	/// Returns pointer to null-term string
	/////////////////////////////////
	char *ArrayToHex(const BYTE *data, unsigned int dataSize, char *hexStr, unsigned int hexStrLen)
	{
		if (hexStr != NULL)
		{
			unsigned int hexLength = (hexStrLen - 1) >> 1;

			if (hexLength < dataSize)
				dataSize = hexLength;

			hexLength = dataSize << 1;

			for (unsigned int i=0; i < dataSize; i++) {
				ByteToChars(*(data+i),hexStr[i*2],hexStr[i*2+1]);
			}

			hexStr[hexLength] = 0;
		}

		return hexStr;    
	}


	std::string ArrayToString(unsigned char *byteArray, unsigned int arraySize) 
	{
		char Hi, Lo;
		std::string result;

		for (UINT byteID=0; byteID< arraySize; byteID++)
		{
			ByteToChars(byteArray[byteID], Hi, Lo);
			result += Hi;
			result += Lo;
		}

		return result;
	}

	/////////////////////////////////
	/// Converts bytes array to String /// {0x02, 0x89, 0x4A, 0XBC } -> "02894ABC" 
	/////////////////////////////////
	CF_ErrorType ByteToHex(UCHAR *bytes, UINT bytesCount, char *hexStr, UINT *hexStrSize) {
		/// Checks input data
		if (bytes == NULL) return ERR_CreateInstance( ERR_NULL_POINTER, 0);
		if (bytesCount == 0) return ERR_CreateInstance( ERR_INVALID_ARGUMENT, 0);
		if (hexStrSize == NULL) return ERR_CreateInstance( ERR_NULL_POINTER, 0);

		if (hexStr == NULL) {
			*hexStrSize = bytesCount*2+1;
			return ErrorOK;
		}

		if (*hexStrSize < bytesCount*2+1) {
			return ERR_CreateInstance( ERR_INVALID_ARGUMENT, 0);
		}

		UINT localBufferSize = bytesCount*2+1;
		char *localBuffer = new char[localBufferSize];
		ZeroMemory(localBuffer,localBufferSize);

		CF_ErrorType error = ErrorOK;
		for (UINT i=0; i<bytesCount; i++) {
			ByteToChars(bytes[i], localBuffer[i*2], localBuffer[i*2+1]);
		}

		if (error.IsOk()) {
			strcpy_s(hexStr,*hexStrSize,localBuffer);
			*hexStrSize = bytesCount*2+1;
		}

		delete [] localBuffer;

		return error;
	}

	void StringToArray(std::string stringArray, unsigned char ** array, unsigned int *arraySize) {

		std::string upString = makeUpper( stringArray );

		*arraySize = stringArray.length() / 2;
		*array = NULL;
		if (*arraySize == 0) return;

		*array = new BYTE[ *arraySize ];
		memset(*array,0x00,*arraySize);

		for (UINT byteID = 0; byteID < *arraySize; byteID++) {

			BYTE H = 0;
			BYTE L = 0;
			if ((upString[ byteID*2 ] >= '0') && (upString[ byteID*2 ] <= '9' )) {
				H = upString[ byteID*2 ] - '0';
			} else if ((upString[ byteID*2 ] >= 'A') && (upString[ byteID*2 ] <= 'F' )) {
				H = upString[ byteID*2 ] - 'A'+0x0A;
			}

			if ((upString[ byteID*2+1 ] >= '0') && (upString[ byteID*2+1 ] <= '9' )) {
				L = upString[ byteID*2+1 ] - '0';
			} else if ((upString[ byteID*2+1 ] >= 'A') && (upString[ byteID*2+1 ] <= 'F' )) {
				L = upString[ byteID*2+1 ] - 'A'+0x0A;
			}

			(*array)[byteID] = H*0x10+L;
		}
	}

	

	std::string& IntToDigitstring(int val, std::string &res)
	{
		res.reserve(8);
		do
		{
			int rest = val % 10;
			res += (char)(0x30 + rest);
			val = val / 10;
		}
		while (val != 0);

		return res;
	}

	
	std::wstring& IntToDigitstring(int val, std::wstring &res)
	{
		res.reserve(8);
		do
		{
			int rest = val % 10;
			res += (wchar_t)(0x30 + rest);
			val = val / 10;
		}
		while (val != 0);

		return res;
	}


	/*
	bool ValidateParameter( std::map<std::string,std::string> &Parameters, std::string ParameterName, bool CanBeEmpty, bool MustHaveExactLength, unsigned int Length ) {

	bool result = true;

	if (Parameters.count(ParameterName) == 0) {
	if (!CanBeEmpty) {
	result = false;
	}
	} else {
	unsigned int ParameterLength = Parameters[ParameterName].length();

	if (ParameterLength == 0) {
	if (!CanBeEmpty) {
	result = false;
	}
	} else {
	if (MustHaveExactLength) {
	if (ParameterLength != Length) {
	result = false;
	}
	} else {
	if (ParameterLength > Length) {
	result = false;
	}
	}
	}
	}

	return result;
	}
	*/
	

#if defined(_WIN32_WINNT) && (WINVER >= _WIN32_WINNT)

	//#ifdef _USE_PATH_FUNCTIONS_
	//////////////////////////////////////////////////////////////////////////
	/// @brief Checks filePath. Just returns if path is absolute or returns combined path if relative 
	//////////////////////////////////////////////////////////////////////////
	std::string CheckPath( std::string PathToCheck, std::string SourcePath ) {

		std::string result = PathToCheck;
		if (!PathIsFull( PathToCheck )) {

			char filename[MAX_PATH+1] = {0};
			strcpy_s( filename,MAX_PATH+1, SourcePath.c_str());

			if (PathAppendA(filename,PathToCheck.c_str())) {
				char dest[MAX_PATH+1] = {0};
				if (PathCanonicalizeA(dest,filename)){
					result = dest;
				}
			}

		}	
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	/// @brief Checks if path exists.
	//////////////////////////////////////////////////////////////////////////
	bool FileExists( std::string FilePath ) {

		bool result = false;

		FILE *f = NULL;
		errno_t error = fopen_s(&f,FilePath.c_str(),"r");
		if (error == 0 ) {
			if (f != NULL) {
				fclose(f);
				result = true;
			}
		}

		return result;
	}
	

#ifdef FILE_VERSION_INCLUDED
	//////////////////////////////////////////////////////////////////////////
	/// @biref Returns version of dll or exe
	//////////////////////////////////////////////////////////////////////////
	FileVersion GetFileVersion( std::string FilePath ) {
		FileVersion version;

		version.VersionWord[0] = 0;
		version.VersionWord[1] = 0;
		version.VersionWord[2] = 0;
		version.VersionWord[3] = 0;

		DWORD size = 0;
		DWORD handle = 0;
		size = GetFileVersionInfoSizeA(FilePath.c_str(), &handle);
		if (size != 0) {
			BYTE* versionInfo = new BYTE[size];
			if (GetFileVersionInfoA(FilePath.c_str(), handle, size, versionInfo)) {

				UINT	len = 0;
				VS_FIXEDFILEINFO*	vsfi = NULL;
				VerQueryValueA(versionInfo, "\\", (void**)&vsfi, &len);
				version.VersionWord[0] = HIWORD(vsfi->dwFileVersionMS);
				version.VersionWord[1] = LOWORD(vsfi->dwFileVersionMS);
				version.VersionWord[2] = HIWORD(vsfi->dwFileVersionLS);
				version.VersionWord[3] = LOWORD(vsfi->dwFileVersionLS);
			}
			delete [] versionInfo;
		}


		return version;
	}
#endif


	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets current process filename
	//////////////////////////////////////////////////////////////////////////
	std::string GetProcessFilename() {
		std::string result;
		char filename[MAX_PATH+1] = {0};
		//GetModuleFileNameExA( GetCurrentProcess(), NULL, filename, MAX_PATH );
		GetModuleFileNameA( (HINSTANCE)GetCurrentProcess(), filename, MAX_PATH );
		result = std::string(filename);
		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets current module filename
	//////////////////////////////////////////////////////////////////////////
	std::string GetModuleFilename() {
		std::string result;
		char filename[MAX_PATH+1] = {0};
		DWORD res = GetModuleFileNameA( (HINSTANCE)&__ImageBase, filename, MAX_PATH );
		if (res != 0) {
			result = std::string(filename);
		} else {
			result = "";
		}
		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	/// @brief Get path without file
	//////////////////////////////////////////////////////////////////////////
	std::string GetPathWithoutFileSpec(std::string FullPath) {

		std::string result = FullPath;
		char filename[MAX_PATH+1] = {0};
		strncpy_s( filename,MAX_PATH+1, FullPath.c_str(), FullPath.length() );

		if (PathRemoveFileSpecA(filename)) {
			result = std::string(filename);
		}
		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	/// @brief IsDirectory
	//////////////////////////////////////////////////////////////////////////
	bool IsDirectory(std::string FilePath ) {
		bool result = false;

		result = ( PathIsDirectoryA( FilePath.c_str() ) == FALSE ) ? false : true;

		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	/// @brief Check if file path is full
	//////////////////////////////////////////////////////////////////////////
	bool PathIsFull( std::string FilePath ) {
		bool result = false;

		if (!PathIsRelativeA( FilePath.c_str() )) {
			result = true;
		}

		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	/// @biref Creates compound path from two parts
	//////////////////////////////////////////////////////////////////////////
	std::string CombinePath( std::string DirPath, std::string FilePath ) {
		std::string result;

		char resultPath[MAX_PATH+1] = {0};
		if (PathCombineA( resultPath, DirPath.c_str(), FilePath.c_str() )) {
			result = resultPath;
		}

		return result;
	}

	//std::wstring FindFileName( std::wstring &name )
	//{
	//	std::wstring res = PathFindFileNameW(name.c_str());
	//	return res;
	//}
	
#endif


	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets buffer length for conversion from UNICODE to UTF-8.
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetUTF8BufferLength( const wchar_t *inputString ) {

		return WideCharToMultiByte(CP_UTF8, 0, inputString, -1, NULL, 0, NULL, NULL);
	}
	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets converted string from UNICODE to UTF-8
	//////////////////////////////////////////////////////////////////////////
	char *GetUFT8Buffer( const wchar_t *inputString ) {

		unsigned int bufferSize = WideCharToMultiByte(CP_UTF8, 0, inputString, -1, NULL, 0, NULL, NULL);
		char *buffer = new char[ bufferSize ];
		memset(buffer, 0x00, bufferSize);
		unsigned int result = WideCharToMultiByte(CP_UTF8, 0, inputString, -1, buffer, bufferSize, NULL, NULL);
		if (result == 0) {
			delete buffer;
			buffer = NULL;
		}
		return buffer;
	}

	//////////////////////////////////////////////////////////////////////////
	/// @brief Converts hex error string to DWORD
	//////////////////////////////////////////////////////////////////////////
	unsigned long GetHexErrorCode( const char *errorHex ) {

		unsigned int hexLength = strlen( errorHex );
		if (hexLength <= 2) {
			return 0;
		}

		unsigned long  result = 0;
		unsigned int id = 0;
		char HexSymbol = 0;
		for (unsigned int charID = hexLength-1; charID >= 2; charID-- ) {
			HexSymbol = errorHex[charID];
			if ((HexSymbol >= '0') && (HexSymbol <= '9')) {
				HexSymbol -= '0';
			} else if ((HexSymbol >= 'A') && (HexSymbol <= 'F')) {
				HexSymbol -= 'A' + 0xA;				
			} else if ((HexSymbol >= 'a') && (HexSymbol <= 'f')) {
				HexSymbol -= 'a' + 0xA;								
			}
			result += (HexSymbol << (4*id++));
		}

		return result;						
	}
}
