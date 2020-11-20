#include "common_includes.h"

#ifndef _CRYPTOGRAPHY_FRMWRK_
#define _CRYPTOGRAPHY_FRMWRK_

namespace Cryptography
{
	typedef unsigned char BYTE;
	typedef unsigned char DoubleKey[16];
	typedef unsigned char DesKey[8];
	typedef DesKey        MacData;

    static const Cryptography::DesKey   zerro8byte;


	union TDES_16 {
		DoubleKey value;
		struct {
			DesKey keyFirst;
			DesKey keySecond;
		};
	};

	class block64_t
	{
	public:
		static const int Len = 8; // 8 bytes = 64 bits
		BYTE d[Len];

		block64_t();
		block64_t(block64_t &);

		BYTE GetBit(int bit);
		void SetBit(int bit);
		void ClearBit(int bit);
		void Clear();

	};

	bool runUnitTests();

	void LeftShiftInArray(BYTE* data, BYTE* newData, size_t dataLen, BYTE shiftRange = 1);	
	void LeftShiftInArray(BYTE* data, size_t dataLen, BYTE shiftRange = 1);

	void ComputeMAC(const DoubleKey &key, BYTE *pbMessage, int msgLen, MacData &mac);
	void ComputeCMAC_Rb64(const TDES_16 &key, const BYTE *pbMessage, size_t msgLen, MacData &mac);

	void Encrypt8(const DoubleKey &EncryptionKey, const DesKey &rnd, DesKey &cyptogram);
	void DecryptRND(const DoubleKey &DecryptionKey, const DesKey &rndCypt, DesKey &rnd);
	void EncryptCBC16(const DoubleKey &EncryptionKey, const DoubleKey &rnd, DoubleKey &cyptogram);
	bool EncryptCBC16(const TDES_16 &key, const BYTE* pbMessage, size_t msgLen, BYTE* pbCyptogram, size_t cyptoLen, const BYTE* initVect = NULL);
	bool DecryptCBC16(const TDES_16 &key, const BYTE* pbCyptogram, size_t cyptoLen, BYTE* pbOutMess, size_t outLen, const BYTE* initVect = NULL);

	void EncryptDES(block64_t *data, block64_t *cipher, block64_t *key);
	void DecryptDES(block64_t *data, block64_t *cipher, block64_t *key);	
}

#endif