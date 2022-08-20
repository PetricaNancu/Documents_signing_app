#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "KeyManager.h"

class Verifier
{
public:
	Verifier();
	~Verifier();

	std::string decryptRSA(std::string coded);
	std::string decryptAES(std::string pass, std::string crypted);
	std::string decryptCombined(std::string criptat);

	bool RSA_Verify(unsigned char* MsgHash, size_t MsgHashLen,
		const char* Msg, size_t MsgLen, bool* Authentic);
	bool verifySignature(std::string plainText, char* signatureBase64);

	std::string randomBytes(int size);

	void build();
};

