#pragma once
#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/ssl.h>

std::string base64_encode(std::string const& s, bool url = false);
std::string base64_encode_pem(std::string const& s);
std::string base64_encode_mime(std::string const& s);

std::string base64_decode(std::string const& s, bool remove_linebreaks = false);
std::string base64_encode(unsigned char const*, unsigned int len, bool url = false);

void b64_SSL_Encode(const unsigned char* buffer, int length, char** base64Text);
size_t calcDecodeLength(const char* b64input);
void b64_SSL_Decode(const char* b64message, unsigned char** buffer, size_t* length);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */

