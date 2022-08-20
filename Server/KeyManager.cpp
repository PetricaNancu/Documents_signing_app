#include "KeyManager.h"

KeyManager* KeyManager::instance = nullptr;
KeyManager * KeyManager::GetInstance()
{
	if (instance == nullptr)
		instance = new KeyManager;

	return instance;
}

void KeyManager::GenerateRSAkeys(std::string filename)
{
	RSA *r = NULL;
	BIGNUM *bn = NULL;
	BIO *bp_public = NULL, *bp_private = NULL;
	std::string Key;

	bn = BN_new();
	if (BN_set_word(bn, RSA_F4) != 1)
		goto free;

	r = RSA_new();		//generam cheia

	if (RSA_generate_key_ex(r, 2048, bn, NULL) != 1) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("Eroare la RSA_generate_key_ex:\t" + err);
		goto free;
	}

	Key = filename; Key.append("public.pem");

	bp_public = BIO_new_file(Key.c_str(), "w+");			//salvam cheia pub
	if (PEM_write_bio_RSAPublicKey(bp_public, r) != 1) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("PEM_write_bio_RSAPublicKey:\t" + err);
		goto free;
	}


	Key = filename; Key.append("private.pem");
	bp_private = BIO_new_file(Key.c_str(), "w+");			//salvam cheia priv
	if (PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL) != 1) {
		std::string err = ERR_error_string(ERR_get_error(), NULL);
		ExceptionsHandler e("PEM_write_bio_RSAPrivateKey:\t" + err);
		goto free;
	}

free:
	BIO_free_all(bp_private);
	BIO_free_all(bp_public);
	RSA_free(r);
	BN_free(bn);

	Key = filename;
	setKey(Key + "public.pem");
	setKey(Key + "private.pem");
}

void KeyManager::setKey(std::string filename)
{
	{
		std::ifstream f; f.open(filename, std::ios::binary);

		if (f.is_open() == true) {
			f.seekg(0, f.end);
			int length = f.tellg();
			f.seekg(0, f.beg);

			char* data = new char[length];
			f.read(data, length);

			const char* key = data;

			BIO* bio = BIO_new_mem_buf((void*)key, -1);
			BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);


			if (filename.find("public.pem") != std::string::npos) {
				this->publKey = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
				if (!publKey) {
					std::string err = ERR_error_string(ERR_get_error(), NULL);
					ExceptionsHandler e("PEM_read_bio_RSAPublicKey:\t" + err);
				}
			}
			else if (filename.find("private.pem") != std::string::npos) {
				this->privKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
				if (!privKey) {
					std::string err = ERR_error_string(ERR_get_error(), NULL);
					ExceptionsHandler e("PEM_read_bio_RSAPrivateKey:\t" + err);
				}
			}
			else {
				ExceptionsHandler e("Nume gresit fisier");
			}
			BIO_free(bio);
			delete[] data;
		}
		else {
			ExceptionsHandler e("Deschiderea fisierului a esuat");
		}
	}
}

void KeyManager::assignKeys(std::string command)
{
	code_veryfier& getter = code_veryfier::getInstance();

	std::string filename = PATH;
	filename.append("\\Use\\Clients\\");
	filename.append(getter.get_username());	filename.append("\\");

	if (command == "register")
		GenerateRSAkeys(filename);

	if (command == "login") {
		setKey(filename + "public.pem");
		setKey(filename + "private.pem");
	}

	//if (getPublKey() == NULL || getPrivKey() == NULL)
		//ExceptionsHandler e("Eroare intampinata la setarea cheilor");
}

RSA * KeyManager::getPublKey()
{
	return this->publKey;
}

RSA * KeyManager::getPrivKey()
{
	return this->privKey;
}

KeyManager::KeyManager()
{
	ERR_load_CRYPTO_strings();
	OpenSSL_add_all_algorithms();
}


KeyManager::~KeyManager()
{
	EVP_cleanup();
	ERR_free_strings();
}

