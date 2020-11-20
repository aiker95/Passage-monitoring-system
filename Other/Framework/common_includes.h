//#include "Framework.conf.h"


//#include <SDKDDKVer.h>
#ifdef UF_CONF_HAS_WINDOWS_H
#  ifdef USE_MFC
#    include <afxwin.h>
#  else
#    include <Windows.h>
#  endif
#else
#	define MAX_DWORD 0xFFFFFFFF
#	define MAX_SHORT 0xFFFF
#	define WAIT_INFINITE	MAX_DWORD
#	define NULL 0
#endif

#include <string>
#include <vector>

typedef std::vector<unsigned char>  CF_ByteArray;


#define SOH 0x01

#define MAKE_SHORT(Hi, Lo) (((Hi)<<8)+(Lo))
#define GET_HI_FROM_SHORT(value) (BYTE)(((value) & 0xFF00)>> 8)
#define GET_LO_FROM_SHORT(value) (BYTE)((value) & 0x00FF)
#define SWAP_BYTES_OF_SHORT(value) (MAKE_SHORT(GET_LO_FROM_SHORT((value)),GET_HI_FROM_SHORT((value))))

#define GET_HI_WORD_FROM_DWORD(value) (((value) & 0xFFFF0000) >> 16)
#define GET_LO_WORD_FROM_DWORD(value) ((value) & 0x0000FFFF)

#define WAIT_IMMEDIATE 	0

#ifndef UF_DEF_ENCODING
#define UF_DEF_ENCODING 1251
#endif