#pragma once

#include "KeyManager.h"
#include "Saver.h"

class Signer
{
public:
	Signer();
	~Signer();

	std::string encryptRSA(std::string msg);
	std::string encryptAES(std::string pass, std::string text);
	std::string encryptCombined(std::string text);

	char* signMessage(std::string plainText);
	bool RSA_Sign(const unsigned char* Msg, size_t MsgLen, unsigned char** EncMsg, size_t* MsgLenEnc);

	std::string randomBytes(int size);
	void build();
	void send_file();
	void recieve_file();
};

