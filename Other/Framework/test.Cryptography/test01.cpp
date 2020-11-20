#include <conio.h>
#include <exception>
#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <windows.h>
#include "..\Framework\Cryptography.h"

using namespace std;


int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"Russian");
  //setlocale(LC_CTYPE, "Russian_Rusian.1251");
    
    if (argc < 1)
    {
        std::cout << "Требуется миниму два параметра" << std::endl
                  << "Возможные параметры:" << std::endl
                  << "  <начальный вид окна> [0..3]" << std::endl;
        
        system("pause");
        return 0;
    }


	//_ASSERT( Cryptography::runUnitTests() );	
	//*

    // ----									F7 52 D2 47 CD 1C 2C 74			--- //
    // ----									6E 3F 4A 23 02 35 01 AE 		--- //
	Cryptography::TDES_16 authMasterKey = {0xF7, 0x52, 0xD2, 0x47, 0xCD, 0x1C, 0x2C, 0x74, 0x6E, 0x3F, 0x4A, 0x23, 0x02, 0x35, 0x01, 0xAE};
	
	Cryptography::DoubleKey cvc;
	Cryptography::DoubleKey zerro = {0, 0, 0, 0, 0, 0, 0, 0};
	Cryptography::EncryptCBC16(authMasterKey, zerro, 8, cvc, 16); // cvc = 72 1b c3 44 72 34 9a 9a

	//unsigned char moduleID_1[] =  {0, 0, 0, 0x11,   0x39, 0x00, 0x12, 0x47};
	//unsigned char moduleID_2[] =  {0, 0, 0, 0x12,   0x39, 0x00, 0x12, 0x47};
	//unsigned char moduleID_1[] =  {0x11, 0x11, 0x11, 0x11,   0x39, 0x00, 0x12, 0x47};
	//unsigned char moduleID_2[] =  {0x12, 0x12, 0x12, 0x12,   0x39, 0x00, 0x12, 0x47};
	//unsigned char moduleID_1[] =  {0x11,  0x39, 0x00, 0x12, 0x47};//,  0, 0, 0 };
	//unsigned char moduleID_2[] =  {0x12,  0x39, 0x00, 0x12, 0x47};//,  0, 0, 0};

	unsigned char moduleID_1[] =  {0x11,  0x39, 0x00, 0x01, 0x07};//,  0, 0, 0};
	unsigned char moduleID_2[] =  {0x12,  0x39, 0x00, 0x01, 0x07};//,  0, 0, 0};

	Cryptography::MacData derivedKeyA;
	Cryptography::MacData derivedKeyB;
	Cryptography::ComputeCMAC_Rb64(authMasterKey, moduleID_1, sizeof(moduleID_1), derivedKeyA);
	Cryptography::ComputeCMAC_Rb64(authMasterKey, moduleID_2, sizeof(moduleID_2), derivedKeyB);

	Cryptography::DoubleKey authKey;
	memcpy(authKey + 0, derivedKeyA, 8);
	memcpy(authKey + 8, derivedKeyB, 8);
	//*/
	//Cryptography::DoubleKey authKey = {0xbe, 0x10, 0x37, 0x67, 0xbd, 0xa9, 0x1e, 0x7f, 0x12, 0xe7, 0xcc, 0x81, 0x9a, 0x20, 0xfe, 0x5c};


	Cryptography::MacData cryptRND_A = {0xAC, 0x5F, 0xC8, 0xDD, 0x4A, 0x84, 0x6B, 0x07};
	Cryptography::MacData rndA;
	Cryptography::DecryptRND(authKey, cryptRND_A, rndA);
	
	Cryptography::TDES_16	authKey2;
	memcpy(authKey2.keyFirst,  derivedKeyA, 8);
	memcpy(authKey2.keySecond, derivedKeyB, 8);
	Cryptography::MacData rndA2;
	Cryptography::DecryptCBC16(authKey2, cryptRND_A, 8, rndA2, 8);

	Cryptography::DoubleKey rndAB = {0x1a, 0xc3, 0xC7, 0x94, 0x37, 0x06, 0x33, 0x5B,  0, 0, 0, 0, 0, 0, 0, 0};
	Cryptography::DoubleKey cryptRND_AB;
	memcpy(rndAB+8, rndA2+1, 8);
	rndAB[15] = rndA2[0];

	Cryptography::EncryptCBC16(authKey, rndAB, cryptRND_AB);

	Cryptography::DoubleKey cryptRND_AB2;
	Cryptography::EncryptCBC16(authKey2, rndAB, 16, cryptRND_AB2, 16);

    // _getch();  
    system("pause");

    return 0;
}

