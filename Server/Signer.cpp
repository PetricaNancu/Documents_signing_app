#include "Signer.h"

std::string Signer::encryptRSA(std::string msg)
{
	KeyManager *key_mng; key_mng = key_mng->GetInstance();
	RSA* pubKey = key_mng->getPublKey();

	int keyLen = RSA_size(pubKey);
	unsigned char* ed = (unsigned char*)malloc(keyLen);

	int resultLen = RSA_public_encrypt(msg.length(), (const unsigned char*)msg.c_str(), ed, pubKey, PADDING);
	if (resultLen == -1) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la RSA_public_encrypt:\t" + err);
		return "";
	}

	std::string buf(reinterpret_cast<char*>(ed), resultLen);
	return buf;
}

std::string Signer::randomBytes(int size)
{
	unsigned char* s = (unsigned char*)malloc(size);
	RAND_bytes(s, size);

	std::string buf(reinterpret_cast<char*>(s), size);
	return buf;
}
void Signer::recieve_file()
{
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\file.txt";
	
	Server_connecter& connecter = Server_connecter::getInstance();

	std::string line;
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
}
void Signer::build()
{
	Server_connecter& connecter = Server_connecter::getInstance();
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\file.txt";

	recieve_file();

	std::ifstream reader;
	reader.open(file_path);
	std::string aux;
	std::string fisier;
	while (std::getline(reader, aux))
	{
		fisier.append(aux);
		fisier.append("\n");
	}
	reader.close();

	char* signature = signMessage(fisier);
	fisier = "#@# Signature:";
	fisier.append(signature);


	std::ofstream writer;
	writer.open(file_path, std::ios_base::app);
	writer << fisier;
	writer.close();


	std::string save = getter.get_save_answer();
	if (save == "save")
	{
		//std::cout << getter.get_save_answer() << "\t" << getter.get_save_name();
		Saver saver(getter.get_save_name());
		saver.save();
	}

	send_file();
	
}
void Signer::send_file()
{
	Server_connecter& connecter = Server_connecter::getInstance();
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\file.txt";

	std::ifstream reader;
	reader.open(file_path);
	std::string message;
	std::string line;
	while (std::getline(reader, line))
	{
		message = connecter.recieve_message();
		line.append("\n");
		if (message == "ok")
			connecter.send_file(line);

	}
	message = connecter.recieve_message();
	if (message == "ok")
		connecter.send_file("#@#@#");

	message = connecter.recieve_message();
	if (message == "done")
	{
		std::cout << "Fisier trimis cu success!" << std::endl;
	}
}

std::string Signer::encryptAES(std::string pass, std::string text)
{
	std::string salt = randomBytes(SALTSIZE);
	int rounds = 1;

	unsigned char key[KEYSIZE];
	unsigned char iv[IVSIZE];

	int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (const unsigned char*)salt.c_str(),
		(const unsigned char*)pass.c_str(), pass.length(), rounds, key, iv);

	if (i != KEYSIZE) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_BytesToKey:\t" + err);
	}

	EVP_CIPHER_CTX  *en = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(en);

	if (!EVP_EncryptInit_ex(en, EVP_aes_256_cbc(), NULL, key, iv)) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_EncryptInit_ex:\t" + err);
	}

	int c_len = text.length() + AES_BLOCK_SIZE;
	int f_len = 0;
	unsigned char* ciphertext = (unsigned char*)malloc(c_len);

	if (!EVP_EncryptUpdate(en, ciphertext, &c_len, (unsigned char*)text.c_str(), text.length())) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_EncryptUpdate:\t" + err);
	}

	if (!EVP_EncryptFinal(en, ciphertext + c_len, &f_len)) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_EncryptFinal:\t" + err);
	}

	int len = c_len + f_len;
	EVP_CIPHER_CTX_cipher(en);

	std::string encrypted = std::string(reinterpret_cast<char*>(ciphertext), len);
	std::string ultim;		ultim.append("Salted__");	ultim.append(salt);		ultim.append(encrypted);

	free(ciphertext);
	return ultim;
}

std::string Signer::encryptCombined(std::string text)
{
	

	std::string AES_pass = randomBytes(8);
	std::string encrypted_AES_Pass = encryptRSA(AES_pass);

	//std::string b64_pass= base64_encode(reinterpret_cast<const unsigned char*>(AES_pass.c_str()), AES_pass.length());
	//std::cout << "Cheia AES criptata: " << b64_pass << "\n\n";

	std::string encrypted = encryptAES(AES_pass, text);

	std::string criptat = "";
	criptat.append(encrypted_AES_Pass);
	criptat.append(encrypted);

	//std::cout << "Text inainte de criptare: " << text << "\n\n";
	//std::cout << "Text dupa criptare: " << criptat << "\n";

	//RSA_free(pubKey);

	return criptat;
}

bool Signer::RSA_Sign(const unsigned char * Msg, size_t MsgLen, unsigned char ** EncMsg, size_t * MsgLenEnc)
{
	KeyManager *key_mng; key_mng = key_mng->GetInstance();
	RSA* privateRSA = key_mng->getPrivKey();

	EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
	EVP_PKEY* priKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(priKey, privateRSA);
	if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestSignInit:\t" + err);
		return false;
	}

	if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestSignUpdate:\t" + err);
		return false;
	}

	if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestSignFinal:\t" + err);
		return false;
	}

	*EncMsg = (unsigned char*)malloc(*MsgLenEnc);
	if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la EVP_DigestSignFinal:\t" + err);
		return false;
	}
	EVP_MD_CTX_free(m_RSASignCtx);
	return true;
}

char * Signer::signMessage(std::string plainText)
{
	
	unsigned char* encMessage;
	char* base64Text;
	size_t encMessageLength;

	RSA_Sign((unsigned char*)plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);

	std::string signature = std::string(reinterpret_cast<char*> (encMessage), encMessageLength);
	signature = encryptCombined(signature);

	b64_SSL_Encode((unsigned char*)signature.c_str(), signature.length(), &base64Text);

	std::string bt = base64Text;
	int pos = bt.find("==");
	if (pos == -1) {
		pos = bt.find("=");
		if (pos != -1)
			base64Text[pos + 1] = '\0';
	}
	else
		base64Text[pos + 2] = '\0';

	free(encMessage);

	//RSA_free(privateRSA);

	return base64Text;
}

Signer::Signer()
{
}


Signer::~Signer()
{
}
