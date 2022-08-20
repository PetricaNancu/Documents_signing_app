#include "Verifier.h"

std::string Verifier::decryptRSA(std::string coded)
{

	KeyManager *key_mng; key_mng = key_mng->GetInstance();
	RSA* privKey = key_mng->getPrivKey();

	int keyLen = RSA_size(privKey);

	unsigned char* ed = (unsigned char*)malloc(keyLen);

	int resultLen = RSA_private_decrypt(keyLen, (const unsigned char*)coded.c_str(), ed, privKey, PADDING);

	if (resultLen == -1) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la RSA_private_decrypt:\t" + err);
		return "";
	}

	std::string buf(reinterpret_cast<char*>(ed), resultLen);

	//RSA_free(privKey);

	return buf;
}

std::string Verifier::decryptAES(std::string pass, std::string crypted)
{
	std::string salt;
	std::string data;

	if (crypted.substr(0, 8) == "Salted__") {
		salt = crypted.substr(8, 8);
		data = crypted.substr(16);
	}
	else {
		ExceptionsHandler e("Nu a fost gasit Salt in stringul criptat");
		salt = randomBytes(SALTSIZE);
	}

	int rounds = 1;
	unsigned char key[KEYSIZE];
	unsigned char iv[IVSIZE];

	int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (const unsigned char*)salt.c_str(),
		(const unsigned char*)pass.c_str(), pass.length(), rounds, key, iv);

	if (i != KEYSIZE) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_BytesToKey:\t" + err);
	}

	EVP_CIPHER_CTX *de;
	de = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(de);

	if (!EVP_DecryptInit_ex(de, EVP_aes_256_cbc(), NULL, key, iv)) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DecryptInit_ex:\t" + err);
	}

	int p_len = data.length();
	int f_len = 0;
	unsigned char* plaintext = (unsigned char*)malloc(p_len + AES_BLOCK_SIZE);

	if (!EVP_DecryptUpdate(de, plaintext, &p_len, (unsigned char*)data.c_str(), data.length())){
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DecryptUpdate:\t" + err);
	}

	if (!EVP_DecryptFinal_ex(de, plaintext + p_len, &f_len)){
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DecryptFinal_ex:\t" + err);
	}

	int len = p_len + f_len;
	EVP_CIPHER_CTX_cleanup(de);

	std::string decrypted = std::string(reinterpret_cast<char*>(plaintext), len);
	free(plaintext);

	return decrypted;
}

std::string Verifier::decryptCombined(std::string criptat)
{
	
	std::string header = "Salted__";
	int pos = criptat.find(header);
	if (pos == -1)
		ExceptionsHandler e("Nu a fost gasit Salt in stringul criptat");

	std::string encryptedPass = criptat.substr(0, pos);
	std::string encrypted = criptat.substr(pos);

	std::string pass = decryptRSA(encryptedPass);

	std::string text = decryptAES(pass, encrypted);

	return text;
}

std::string Verifier::randomBytes(int size)
{
	unsigned char* s = (unsigned char*)malloc(size);
	RAND_bytes(s, size);

	std::string buf(reinterpret_cast<char*>(s), size);
	return buf;
}

void Verifier::build()
{
	code_veryfier& getter = code_veryfier::getInstance();
	Server_connecter& connecter = Server_connecter::getInstance();

	std::string line;
	std::string file_path = getter.get_path() + "\\file.txt";
	std::ofstream creator;
	creator.open(file_path);
	creator.close();
	creator.open(file_path, std::ios_base::app);

	do
	{
		connecter.send_message("ok");
		line = connecter.recieve_message();
		if (line == "#@#@#")
		{
			creator.close();
			connecter.send_message("done");
		}
		else
			creator << line;
	} while (line != "#@#@#");

	creator.close();

	std::ifstream reader;
	reader.open(file_path);
	std::string aux;
	std::string fisier;
	while (std::getline(reader, aux))
	{
		fisier.append(aux);
		fisier.append("\n");
	}
	fisier.substr(fisier.length() - 2);
	reader.close();

	
	int pos = fisier.find("#@# Signature:");
	std::string text = fisier.substr(0, pos);
	//text.resize(text.size() - 1);

	std::string ssign = fisier.substr(pos + 14);
	ssign.resize(ssign.size() - 1);
	char* signature;
	signature = new char[ssign.length() + 1];
	strcpy(signature, ssign.c_str());

	bool authentic = verifySignature(text, signature);
	if (authentic) {
		std::cout << "\n\nAuthentic" << std::endl;
		connecter.send_message("corect");
	}
	else {
		std::cout << "\n\nNot Authentic" << std::endl;
		connecter.send_message("incorect");
	}
	delete[] signature;
}

bool Verifier::RSA_Verify(unsigned char * MsgHash, size_t MsgHashLen, const char * Msg, size_t MsgLen, bool * Authentic)
{
	KeyManager *key_mng; key_mng = key_mng->GetInstance();
	RSA* publicRSA = key_mng->getPublKey();


	*Authentic = false;
	EVP_PKEY* pubKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pubKey, publicRSA);
	EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

	if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestVerifyInit:\t" + err);
		return false;
	}

	if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestVerifyUpdate:\t" + err);
		return false;
	}


 	int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
	if (AuthStatus == 1) {
		*Authentic = true;
		EVP_MD_CTX_free(m_RSAVerifyCtx);
		return true;
	}
	else if (AuthStatus == 0) {
		*Authentic = false;
		EVP_MD_CTX_free(m_RSAVerifyCtx);
		return true;
	}
	else {
		*Authentic = false;
		EVP_MD_CTX_free(m_RSAVerifyCtx);
		return false;
	}

	//RSA_free(publicRSA);
}

bool Verifier::verifySignature(std::string plainText, char * signatureBase64)
{
	
	unsigned char* encMessage;
	size_t encMessageLength;
	bool authentic;

	b64_SSL_Decode(signatureBase64, &encMessage, &encMessageLength);

	std::string en = std::string(reinterpret_cast<char*>(encMessage), encMessageLength);
	en = decryptCombined(en);

	encMessage = (unsigned char*)en.c_str();
	encMessageLength = en.length();

	bool result = RSA_Verify(encMessage, encMessageLength, plainText.c_str(), plainText.length(), &authentic);

	return result & authentic;
}

Verifier::Verifier()
{
}


Verifier::~Verifier()
{
}
