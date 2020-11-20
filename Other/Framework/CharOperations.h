#ifndef _CHAR_OPERATIONS_H_
#define _CHAR_OPERATIONS_H_

namespace UFramework {

	//////////////////////////////////////////////////////////////////////////
	/// @brief Class containes operations on characters
	//////////////////////////////////////////////////////////////////////////
	class CharOperations {
	public:
		//////////////////////////////////////////////////////////////////////////
		/// @brief isHex - проверяет байт на принадлежность диапазону '0'-'9','a'-'f','A'-'F'
		///
		/// @param[in] character - проверяемый байт
		///
		/// @return true при успехе и false, если символ не в диапазоне
		//////////////////////////////////////////////////////////////////////////
		static bool isHex( unsigned char character ) {
			if (! (
				( (character >= '0') && (character <= '9') ) ||
				( (character >= 'a') && (character <= 'f') ) ||
				( (character >= 'A') && (character <= 'F') ) ) )
			{
				return false;
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		/// @brief isNumber - проверяет байт на принадлежность диапазону '0'-'9'
		///
		/// @param[in] character - проверяемый байт
		///
		/// @return true при успехе и false, если символ не в диапазоне
		//////////////////////////////////////////////////////////////////////////
		static bool isNumber( unsigned char character ) {
			if (!
				( (character >= '0') && (character <= '9') ) )
			{
				return false;
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		/// @brief isAlphaNumber - проверяет байт на принадлежность диапазону '0'-'9','a'-'z','A'-'Z'
		///
		/// @param[in] character - проверяемый байт
		///
		/// @return true при успехе и false, если символ не в диапазоне
		//////////////////////////////////////////////////////////////////////////
		static bool isAlphaNumber( unsigned char character ) {
			if (! (
				( (character >= '0') && (character <= '9') ) ||
				( (character >= 'a') && (character <= 'z') ) ||
				( (character >= 'A') && (character <= 'Z') ) ))
			{
				return false;
			}
			return true;	
		}
		//////////////////////////////////////////////////////////////////////////
		/// @brief isOneZero - проверяет байт на принадлежность диапазону '0'-'1'
		///
		/// @param[in] character - проверяемый байт
		///
		/// @return true при успехе и false, если символ не в диапазоне
		//////////////////////////////////////////////////////////////////////////
		static bool isOneZero( unsigned char character ) {
			if (!
				( (character >= '0') && (character <= '1') ) )
			{
				return false;
			}
			return true;	
		}
		//////////////////////////////////////////////////////////////////////////
		/// @brief hexDigitToChar - преобразовывает HEX-цифру в символ
		/// Старший полубайт игнорируется
		///
		/// @param[in] hexDigit - HEX-цифра
		/// 
		/// @return Символ
		//////////////////////////////////////////////////////////////////////////
		static char hexDigitToChar( unsigned char hexDigit ) {

			char Char = hexDigit & 0x0F;
			if ((Char >= 0x00) && (Char <= 0x09)) {
				Char += '0';
			} else if ((Char >= 0x0A) && (Char <= 0x0F)) {
				Char += 'A'-0x0a;
			}  else {
				Char = '0';
			}

			return Char;
		}

		//////////////////////////////////////////////////////////////////////////
		/// @brief charToHexDigit - преобразует символ в число
		///	Возвращается только полубайт
		/// 
		/// @param[in] character - символ '0'-'9','a'-'f','A'-'F'
		///
		/// @return HEX-цифра
		//////////////////////////////////////////////////////////////////////////
		static unsigned char charToHexDigit( char character ) {

			unsigned char result = 0;

			if ((character >= '0') && (character <= '9')) {
				result = character - '0';
			} else if ((character >= 'a') && (character <= 'f')) {
				result = character - 'a'+0x0a;
			} else if ((character >= 'A') && (character <= 'F')) {
				result = character - 'A'+0x0a;
			}

			return result;
		}

	};

};

#endif // #ifndef _CHAR_OPERATIONS_H_