#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "ExceptionsHandler.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2715"

class Server_connecter
{
private:
	static Server_connecter* instance;
	Server_connecter() {}
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	int iResult;
public:


	static Server_connecter& getInstance();
	void initialize_socket();
	void accept_socket();
	void close_socket();
	void send_message(std::string message);
	void send_file(std::string message);
	std::string recieve_message();
	int get_iResult() { return this->iResult; }

	~Server_connecter() {};
};

