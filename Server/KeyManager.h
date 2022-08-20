#pragma once
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>

#include "code_veryfier.h"
#include "Base64.h"

#define PADDING RSA_PKCS1_PADDING
#define KEYSIZE 32
#define IVSIZE 32
#define BLOCKSIZE 256
#define SALTSIZE 8

#define PATH "M:\\7--------------QT------------------\\Server_nou\\Server"

class KeyManager
{
private:
	static KeyManager *instance;

	RSA* publKey;
	RSA* privKey;
	

public:
	KeyManager();
	~KeyManager();

	void GenerateRSAkeys(std::string filename);
	void setKey(std::string filename);
	void assignKeys(std::string command);

	RSA* getPublKey();
	RSA* getPrivKey();


	static KeyManager *GetInstance();
	KeyManager& operator=(const KeyManager&);
};

