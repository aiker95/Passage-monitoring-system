#include "Cryptography.h"
#include <stdio.h>
#include <memory.h>

using namespace Cryptography;


BYTE PC1[] = {56, 48, 40, 32, 24, 16,  8,
0, 57, 49, 41, 33, 25, 17,
9,  1, 58, 50, 42, 34, 26,
18, 10,  2, 59, 51, 43, 35,
62, 54, 46, 38, 30, 22, 14,
6, 61, 53, 45, 37, 29, 21,
13,  5, 60, 52, 44, 36, 28,
20, 12,  4, 27, 19, 11,  3};

BYTE rot_table[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

BYTE PC2[] = {13, 16, 10, 23,  0,  4,
2, 27, 14,  5, 20,  9,
22, 18, 11,  3, 25,  7,
15,  6, 26, 19, 12,  1,
40, 51, 30, 36, 46, 54,
29, 39, 50, 44, 32, 47,
43, 48, 38, 55, 33, 52,
45, 41, 49, 35, 28, 31};

BYTE FIP[] = {57, 49, 41, 33, 25, 17,  9,  1,
59, 51, 43, 35, 27, 19, 11,  3,
61, 53, 45, 37, 29, 21, 13,  5,
63, 55, 47, 39, 31, 23, 15,  7,
56, 48, 40, 32, 24, 16,  8,  0,
58, 50, 42, 34, 26, 18, 10,  2,
60, 52, 44, 36, 28, 20, 12,  4,
62, 54, 46, 38, 30, 22, 14,  6};

BYTE IIP[] = {39,  7, 47, 15, 55, 23, 63, 31,
38,  6, 46, 14, 54, 22, 62, 30,
37,  5, 45, 13, 53, 21, 61, 29,
36,  4, 44, 12, 52, 20, 60, 28,
35,  3, 43, 11, 51, 19, 59, 27,
34,  2, 42, 10, 50, 18, 58, 26,
33,  1, 41,  9, 49, 17, 57, 25,
32,  0, 40,  8, 48, 16, 56, 24};

BYTE EBit[] = {31,  0,  1,  2,  3,  4,
3,  4,  5,  6,  7,  8,
7,  8,  9, 10, 11, 12,
11, 12, 13, 14, 15, 16,
15, 16, 17, 18, 19, 20,
19, 20, 21, 22, 23, 24,
23, 24, 25, 26, 27, 28,
27, 28, 29, 30, 31,  0};

BYTE SBox[8][4][16] = 
{
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
	},

	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
	},

	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
	},

	{
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
	},

	{
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
	},

	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
	},

	{
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
	},

	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
	},
};

BYTE PT[] = {15,  6, 19, 20, 
28, 11, 27, 16,
0, 14, 22, 25, 
4, 17, 30,  9,
1,  7, 23, 13,
31, 26,  2,  8,
18, 12, 29,  5,
21, 10,  3, 24};

namespace Cryptography
{
	block64_t::block64_t()
	{
		Clear();
	}

	block64_t::block64_t(block64_t &bl)
	{
		memcpy(d, bl.d, 8);

	}

	// all bits are addressed from 0 to 63
	BYTE block64_t::GetBit(int bit)
	{
		if (bit>=64) return 0;

		if (d[bit/8] & (1 << (7 - bit%8))) return 1;
		else  return 0;
	}

	void block64_t::SetBit(int bit)
	{
		if (bit>=64) return;

		d[bit/8] |= (1 << (7 - bit%8));
	}

	void block64_t::ClearBit(int bit)
	{
		if (bit>=64) return;

		BYTE tmp = 0xFF ^ (1 << (7 - bit%8));

		d[bit/8] &= tmp;
	}

	void block64_t::Clear()
	{
		memset(d, 0, 8);
	}
}

void PermutedChoise1(block64_t *key, block64_t *keypc)
{
	keypc->Clear();

	for (int i=0; i<=63; i++)
	{
		// look up for the bit
		int bit=0;
		while (bit<56)
		{
			if (PC1[bit] == i) break;
			bit++;
		}

		if ((bit != 56) && (key->GetBit(i)))
			keypc->SetBit(bit);
	}
}

void PermutedChoise2(block64_t *key, block64_t *keypc)
{
	keypc->Clear();

	for (int i=0; i<=63; i++)
	{
		// look up for the bit
		int bit=0;
		while (bit<48)
		{
			if (PC2[bit] == i) break;
			bit++;
		}

		if ((bit != 48) && (key->GetBit(i)))
			keypc->SetBit(bit);
	}
}

void InitialPermutation(block64_t *in, block64_t *out)
{
	out->Clear();

	for (int i=0; i<=63; i++)
	{
		// look up for the bit
		int bit=0;
		while (bit<64)
		{
			if (FIP[bit] == i) break;
			bit++;
		}

		if ((bit != 64) && (in->GetBit(i)))
			out->SetBit(bit);
	}
}

void FinalPermutation(block64_t *in, block64_t *out)
{
	out->Clear();

	for (int i=0; i<=63; i++)
	{
		// look up for the bit
		int bit=0;
		while (bit<64)
		{
			if (IIP[bit] == i) break;
			bit++;
		}

		if ((bit != 64) && (in->GetBit(i)))
			out->SetBit(bit);
	}
}


void EBitSelection(block64_t *in, block64_t *out)
{
	out->Clear();

	for (int i=0; i<48; i++)
		if (in->GetBit(EBit[i])) out->SetBit(i);
}

void PPermutation(block64_t *in, block64_t *out)
{
	out->Clear();

	for (int i=0; i<32; i++)
		if (in->GetBit(PT[i])) out->SetBit(i);
}


void KeyScheduling(block64_t *key, block64_t *K)
{
	////////////////////////////////////////
	// Permuted Choise 1
	block64_t keypc1;
	PermutedChoise1(key, &keypc1);

	////////////////////////////////////////
	// Key Scheduling
	block64_t RKey, LKey, kjoined;

	// Split to L and R
	int i = 0;
	for (i=0; i<28; i++)
	{
		if (keypc1.GetBit(i)) LKey.SetBit(i);
		if (keypc1.GetBit(i+28)) RKey.SetBit(i);
	}

	for (int R=0; R<16; R++)
	{
		// Rotate L and R and save back to RKey and LKey
		block64_t rotL, rotR;
		for (i=0; i<28; i++)
		{
			int bit = (i + 28 - rot_table[R]) % 28;
			if (LKey.GetBit(i)) rotL.SetBit(bit);
			if (RKey.GetBit(i)) rotR.SetBit(bit);
		}
		for (i=0; i<8; i++)
		{
			LKey.d[i] = rotL.d[i];
			RKey.d[i] = rotR.d[i];
		}

		// Join back to kjoined
		kjoined.Clear();
		for (i=0; i<28; i++)
		{
			if (LKey.GetBit(i)) kjoined.SetBit(i);
			if (RKey.GetBit(i)) kjoined.SetBit(i+28);
		}

		// Permuted Choise 2, save at K[R]
		PermutedChoise2(&kjoined, &K[R]);
	}


}

void DESCore(block64_t *data, block64_t *cipher, block64_t *K)
{
	block64_t iperm;

	////////////////////////////////////////
	// Data Preparataion
	InitialPermutation(data, &iperm);

	////////////////////////////////////////
	// Split data to L and R
	block64_t LData, RData;
	int i = 0;
	for (i=0; i<32; i++)
	{
		if (iperm.GetBit(i)) LData.SetBit(i);
		if (iperm.GetBit(i+32)) RData.SetBit(i);
	}

	////////////////////////////////////////
	// DES Core function 
	block64_t L[17], R[17], ER, tmp, B[8], A, C;

	// copy L and R to L[0] and R[0]
	for (i=0; i<8; i++)
	{
		L[0].d[i] = LData.d[i];
		R[0].d[i] = RData.d[i];
	}

	// 16 rounds of DES
	for (int I=0; I<16; I++)
	{
		// E-bit Selection
		EBitSelection(&R[I], &ER);

		// xor (ER[I], K[I])
		tmp.Clear();
		for (i=0; i<6; i++)
			tmp.d[i] = ER.d[i] ^ K[I].d[i];

		// Split into 8 6-bit blocks
		for (i=0; i<8; i++)
		{
			B[i].Clear();
			for (int j=0; j<6; j++)
			{
				if (tmp.GetBit(j + 6*i)) B[i].SetBit(j);
			}
		}

		// Using S-Boxes
		A.Clear();
		for (i=0; i<8; i++)
		{
			BYTE row = B[i].GetBit(0) * 2 + B[i].GetBit(5);
			BYTE column = B[i].GetBit(1) * 8 + 
				B[i].GetBit(2) * 4 +
				B[i].GetBit(3) * 2 +
				B[i].GetBit(4);

			BYTE b = SBox[i][row][column];

			if (b & 0x08) A.SetBit(0 + 4*i);
			if (b & 0x04) A.SetBit(1 + 4*i);
			if (b & 0x02) A.SetBit(2 + 4*i);
			if (b & 0x01) A.SetBit(3 + 4*i);
		}

		// P-Permutation
		PPermutation(&A, &C);

		// Calculate R[I+1], L[I+1]

		// xor (C, L[I])
		R[I+1].Clear();
		for (i=0; i<6; i++)
			R[I+1].d[i] = C.d[i] ^ L[I].d[i];

		// copy R[I] to L[I+1]
		for (i=0; i<8; i++)
			L[I+1].d[i] = R[I].d[i];

	}

	// Join R[16] and L[16]
	block64_t preout;
	for (i=0; i<32; i++)
	{
		if (R[16].GetBit(i)) preout.SetBit(i);
		if (L[16].GetBit(i)) preout.SetBit(i+32);
	}

	////////////////////////////////////////
	// Cypher Preparataion
	FinalPermutation(&preout, cipher);
}


void Cryptography::EncryptDES(block64_t *data, block64_t *cipher, block64_t *key)
{
	cipher->Clear();

	block64_t K[16];
	KeyScheduling(key, K);

	DESCore(data, cipher, K);
}

void Cryptography::DecryptDES(block64_t *data, block64_t *cipher, block64_t *key)
{
	block64_t iperm, K[16], IK[16];
	KeyScheduling(key, K);

	// change the keys order
	for (int i=0; i<16; i++)
		for (int j=0; j<8; j++) IK[i].d[j] = K[15-i].d[j];

	DESCore(cipher, data, IK);
}


void Cryptography::ComputeMAC(const DoubleKey &key, BYTE *pbMessage, int msgLen, MacData &mac)
{
	block64_t data;
	BYTE     *pCurrData = pbMessage;
	BYTE     *pEndData = pbMessage + msgLen;
	block64_t key1;
	block64_t key2;
	block64_t cipher1;
	block64_t cipher2;
	block64_t cipher3;

	///////////////////////////////////////////////
	// Retrieve Data
	memcpy(key1.d,  key + 0, 8);
	memcpy(key2.d,  key + 8, 8);

	//memcpy(data.d,  pCurrData, 8);
	for(int i=0;i<8;i++) {
		if (pCurrData < pEndData)
			data.d[i] = 0 ^ *(pCurrData++); // Init value 00 00 00 00 00 00 00 00
		else
			data.d[i] = 0;
	}

	while ( pCurrData < pEndData ) {
		///////////////////////////////////////////////
		// DES Encrypt
		EncryptDES(&data,   &cipher1, &key1);

		for(int i=0;i<8;i++) {
			if (pCurrData < pEndData)
				data.d[i] = cipher1.d[i] ^ *(pCurrData++);
			else
				data.d[i] = cipher1.d[i] ^ 0;
		}
	}


	///////////////////////////////////////////////
	// 3DES Encrypt
	EncryptDES(&data,   &cipher1, &key1);
	DecryptDES(&cipher2, &cipher1, &key2);
	EncryptDES(&cipher2, &cipher3, &key1);

	memcpy(mac, cipher3.d, 8);
}


void Cryptography::EncryptCBC16(const DoubleKey &EncryptionKey, const DoubleKey &rnd, DoubleKey &cyptogram)
{

	/////////////// test desnec ///////////////////
	block64_t data1;
	block64_t data2;
	block64_t key1;
	block64_t key2;
	block64_t cipher1;
	block64_t cipher2;
	block64_t cipher3;
	block64_t cipher4;

	///////////////////////////////////////////////
	// Retrieve Data
	memcpy(data1.d, rnd, 8);
	memcpy(data2.d, rnd+8, 8);
	memcpy(key1.d,  EncryptionKey + 0, 8);
	memcpy(key2.d,  EncryptionKey + 8, 8);

	///////////////////////////////////////////////
	// 3DES Encrypt
	EncryptDES(&data1,   &cipher1, &key1);
	DecryptDES(&cipher2, &cipher1, &key2);
	EncryptDES(&cipher2, &cipher3, &key1);
		
	for(int i=0;i<8;i++)
		data2.d[i] = cipher3.d[i] ^ data2.d[i];

	EncryptDES(&data2,   &cipher1, &key1);
	DecryptDES(&cipher2, &cipher1, &key2);
	EncryptDES(&cipher2, &cipher4, &key1);
		
	memcpy(cyptogram, cipher3.d, 8);
	memcpy(cyptogram+8, cipher4.d, 8);
}


void Cryptography::DecryptRND(const DoubleKey &DecryptionKey, const DesKey &rndCypt, DesKey &rnd)
{
	/////////////// test desnec ///////////////////
	block64_t data1;
	block64_t key1;
	block64_t key2;
	block64_t cipher1;
	block64_t cipher2;
	block64_t cipher3;

	///////////////////////////////////////////////
	// Retrieve Data
	memcpy(data1.d, rndCypt, 8);
	memcpy(key1.d,  DecryptionKey + 0, 8);
	memcpy(key2.d,  DecryptionKey + 8, 8);

	///////////////////////////////////////////////
	// 3DES Decrypt
	DecryptDES(&cipher2, &data1,   &key1);
	EncryptDES(&cipher2, &cipher1, &key2);
	DecryptDES(&cipher3, &cipher1, &key1);

	memcpy(rnd, cipher3.d, 8);
}



void Cryptography::LeftShiftInArray(BYTE* data, BYTE* newData, size_t dataLen, BYTE shiftRange)
{
	BYTE mask = 0x00;

	switch (shiftRange) {
		case 8: mask |= 0x01;
		case 7: mask |= 0x02;
		case 6: mask |= 0x04;
		case 5: mask |= 0x08;
		case 4: mask |= 0x10;
		case 3: mask |= 0x20;
		case 2: mask |= 0x40;
		case 1: mask |= 0x80;
			break;
		default:
			return;
	}

	BYTE shiftingBits = 0;
	for (int i = dataLen-1; i >= 0; --i) 
	{
		BYTE temp = data[i];
		newData[i] = (temp << shiftRange) | shiftingBits;
		shiftingBits = (temp & mask) >> (8-shiftRange);
	}
}


void Cryptography::LeftShiftInArray(BYTE* data, size_t dataLen, BYTE shiftRange)
{
	Cryptography::LeftShiftInArray(data, data, dataLen, shiftRange);
}



void Cryptography::ComputeCMAC_Rb64(const TDES_16 &key, const BYTE* pbMessage, size_t msgLen, MacData &mac)
{
	block64_t initZero;
	memset(initZero.d, 0, block64_t::Len);

	block64_t key1;
	block64_t key2;
	block64_t cipherStep1;
	block64_t cipherStep2;
	block64_t cipherStep3;

	///////////////////////////////////////////////
	// Retrieve Data

	memcpy(key1.d,  key.keyFirst, key1.Len);
	memcpy(key2.d,  key.keySecond, key2.Len);

	///////////////////////////////////////////////
	// 3DES Encrypt
	EncryptDES(&initZero, &cipherStep1, &key1);
	DecryptDES(&cipherStep2, &cipherStep1, &key2);
	EncryptDES(&cipherStep2, &cipherStep3, &key1);
	
	block64_t subKey1;
	block64_t subKey2;

	LeftShiftInArray(cipherStep3.d, subKey1.d, block64_t::Len);
	if (cipherStep3.d[0] & 0x80)
		subKey1.d[block64_t::Len-1] ^= 0x1B;
	
	LeftShiftInArray(subKey1.d, subKey2.d, block64_t::Len);
	if (subKey1.d[0] & 0x80)
		subKey2.d[block64_t::Len-1] ^= 0x1B;


	block64_t	data;
	block64_t	currRes;
	size_t		iterCount = msgLen >> 3; // = msgLen / 8;
	size_t		restBlk   = msgLen - (iterCount << 3); // = msgLen - (iterCount*8);

	if (0 == msgLen)
	{
		iterCount = 1;
		restBlk   = 8;
	}
	else if (restBlk)
		++iterCount;

	memset(currRes.d, 0, block64_t::Len);
	for (size_t iter = 0; iter < iterCount; ++iter)
	{
		if ( (iter+1) >= iterCount)
		{
			const BYTE* msgPtr = pbMessage + (iter << 3);
			BYTE* subKey = restBlk ? subKey2.d : subKey1.d; 
			restBlk = block64_t::Len - restBlk;
			for (size_t i = 0; i < block64_t::Len; ++i)
			{
				if (i == restBlk) {
					data.d[i] = 0x80;
				}
				else if (i < restBlk) {
					data.d[i] = *msgPtr;
					++msgPtr;
				}
				else				  
					data.d[i] = 0x00;

				data.d[i] ^= currRes.d[i] ^ subKey[i];		
			}
		}
		else
		{
			const BYTE* msgPtr = pbMessage + (iter << 3);
			for (size_t i = 0; i < block64_t::Len; ++i)
			{
				data.d[i] = *msgPtr ^ currRes.d[i];
				++msgPtr;
			}
		}

		EncryptDES(&data, &cipherStep1, &key1);
		DecryptDES(&cipherStep2, &cipherStep1, &key2);
		EncryptDES(&cipherStep2, &currRes, &key1);
	}


	for (size_t i = 0; i < block64_t::Len; ++i)
		mac[i] = currRes.d[i];
}


const Cryptography::DesKey   zerro8byte = {0, 0, 0, 0, 0, 0, 0, 0};

bool Cryptography::EncryptCBC16(const TDES_16 &key, const BYTE* pbMessage, size_t msgLen, BYTE* pbCyptogram, size_t cyptoLen, const BYTE* initVect /*= NULL*/)
{
	if  (NULL == pbMessage || 0 == msgLen ) return false;

	size_t outLen = (msgLen & 7) ? ((msgLen >> 3) + 1) << 3 : (msgLen >> 3) << 3;

	if ( NULL == pbCyptogram || cyptoLen < outLen ) return false;

	const BYTE*		currentData  = pbMessage;
	const BYTE*		endOfData    = pbMessage + msgLen; 
	const BYTE*		maskedVector = NULL != initVect ? initVect : zerro8byte;
	BYTE*			currentOut   = pbCyptogram;
	block64_t		chank;
	block64_t		key1;
	block64_t		key2;
	block64_t		cipher1;
	block64_t		cipher2;
	block64_t		cipher3;

	///////////////////////////////////////////////
	memcpy(key1.d,  key.keyFirst, 8);
	memcpy(key2.d,  key.keySecond, 8);
	
	while ( currentData < endOfData )
	{
		for(int i=0; i < block64_t::Len; ++i) 
		{
			if (currentData < endOfData)
				chank.d[i] = maskedVector[i] ^ *(currentData++);
			else
				chank.d[i] = maskedVector[i] ^ 0;
		}
				
		// 3DES Encrypt
		EncryptDES(&chank,   &cipher1, &key1);
		DecryptDES(&cipher2, &cipher1, &key2);
		EncryptDES(&cipher2, &cipher3, &key1);
		maskedVector = cipher3.d;
		memcpy(currentOut, cipher3.d, cipher3.Len);
		currentOut += cipher3.Len;
	}
}


bool Cryptography::DecryptCBC16(const TDES_16 &key, const BYTE* pbCyptogram, size_t cyptoLen, BYTE* pbOutMess, size_t outLen, const BYTE* initVect /*= NULL*/)
{
	if  (NULL == pbCyptogram || 0 == cyptoLen || (cyptoLen & 7) != 0) return false;
	if ( NULL == pbOutMess || outLen < cyptoLen ) return false;

	const BYTE*		currentData  = pbCyptogram;
	const BYTE*		endOfData    = pbCyptogram + cyptoLen; 
	const BYTE*		maskedVector = NULL != initVect ? initVect : zerro8byte;
	BYTE*			currentOut   = pbOutMess;
	block64_t		chank;
	block64_t		key1;
	block64_t		key2;
	block64_t		cipher1;
	block64_t		cipher2;
	block64_t		cipher3;

	///////////////////////////////////////////////
	memcpy(key1.d,  key.keyFirst, 8);
	memcpy(key2.d,  key.keySecond, 8);
	
	while ( currentData < endOfData )
	{
		
		memcpy(chank.d, currentData, chank.Len);

		// 3DES Decrypt
		DecryptDES(&cipher1, &chank,   &key1);
		EncryptDES(&cipher1, &cipher2, &key2);
		DecryptDES(&cipher3, &cipher2, &key1);

		for(int i=0; i < block64_t::Len; ++i)
				currentOut[i] = cipher3.d[i] ^ maskedVector[i];
		maskedVector =  currentData;
		currentData += block64_t::Len;
		currentOut += block64_t::Len;
	}
}


bool Cryptography::runUnitTests()
{
	// ----------------------------------------------------------------------------------- //
	// C-MAC generation on TDES algorithm
	// ----------------------------------------------------------------------------------- //
	{
		Cryptography::TDES_16   master =   {0x4c, 0xf1, 0x51, 0x34,  0xa2, 0x85, 0x0d, 0xd5,
											0x8a, 0x3d, 0x10, 0xba,  0x80, 0x57, 0x0d, 0x38};
		Cryptography::MacData ciphK;

		Cryptography::ComputeCMAC_Rb64(master, NULL, 0, ciphK);
		// bd2ebf9a 3ba00361

		Cryptography::MacData   testData1 = {0, 0, 0, 0, 0, 0, 0, 0};
		Cryptography::ComputeCMAC_Rb64(master, testData1, sizeof(testData1), ciphK);
		// 5f 5c f9 88 9a f7 26 d2
	  
		Cryptography::MacData   testData2 = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96};
		Cryptography::ComputeCMAC_Rb64(master, testData2, sizeof(testData2), ciphK);
		//4ff2ab81 3c53ce83

		Cryptography::BYTE   testData3[] =  {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
											 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
											 0xae, 0x2d, 0x8a, 0x57};
		Cryptography::ComputeCMAC_Rb64(master, testData3, sizeof(testData3), ciphK);
		// 62dd1b47 1902bd4e

		Cryptography::BYTE   testData4[] =  {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
											 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
											 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
											 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51};
		Cryptography::ComputeCMAC_Rb64(master, testData4, sizeof(testData4), ciphK);
		// 31b1e431 dabc4eb8
	}


	// ----------------------------------------------------------------------------------- //
	// CBC-TDES (Encryption) 
	// ----------------------------------------------------------------------------------- //
	{
		Cryptography::TDES_16 keyBundle =   {0x01, 0x23, 0x45, 0x67,  0x89, 0xAB, 0xCD, 0xEF, 
											 0x23, 0x45, 0x67, 0x89,  0xAB, 0xCD, 0xEF, 0x01};

		Cryptography::DesKey  initVect  =   {0xF6, 0x9F, 0x24, 0x45,  0xDF, 0x4F, 0x9B, 0x17};

		Cryptography::BYTE	testData1[] =   {0x6B, 0xC1, 0xBE, 0xE2,  0x2E, 0x40, 0x9F, 0x96, 
											 0xE9, 0x3D, 0x7E, 0x11,  0x73, 0x93, 0x17, 0x2A, 
											 0xAE, 0x2D, 0x8A, 0x57,  0x1E, 0x03, 0xAC, 0x9C, 
											 0x9E, 0xB7, 0x6F, 0xAC,  0x45, 0xAF, 0x8E, 0x51};

		Cryptography::BYTE	testData2[] =   {0x74, 0x01, 0xCE, 0x1E,  0xAB, 0x6D, 0x00, 0x3C,
											 0xAF, 0xF8, 0x4B, 0xF4,  0x7B, 0x36, 0xCC, 0x21, 
											 0x54, 0xF0, 0x23, 0x8F,  0x9F, 0xFE, 0xCD, 0x8F, 
											 0x6A, 0xCF, 0x11, 0x83,  0x92, 0xB4, 0x55, 0x81};

		const size_t msgLen = sizeof(testData1);
		Cryptography::BYTE	testOut1[msgLen];

		bool res = EncryptCBC16(keyBundle, testData1, msgLen, testOut1, msgLen, initVect);
		for (size_t i = 0; res && i < msgLen; ++i)
		{
			res = testOut1[i] == testData2[i];
		}
		if ( !res ) return res;

		memset(testOut1, 0, msgLen);

		res = DecryptCBC16(keyBundle, testData2, msgLen, testOut1, msgLen, initVect);
		for (size_t i = 0; res && i < msgLen; ++i)
		{
			res = testOut1[i] == testData1[i];
		}
		if ( !res ) return res;

		
		Cryptography::BYTE	testOut2[msgLen];
		res = EncryptCBC16(keyBundle, testData1, msgLen, testOut1, msgLen);
		if ( !res ) return res;
		res = DecryptCBC16(keyBundle, testOut1, msgLen, testOut2, msgLen);
		for (size_t i = 0; res && i < msgLen; ++i)
		{
			res = testOut2[i] == testData1[i];
		}
		if ( !res ) return res;
	}

	return true;
}