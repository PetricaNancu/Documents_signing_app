#ifndef CLIENT_CONNECTER_H
#define CLIENT_CONNECTER_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>

//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2715"

class Client_connecter
{

private:
    static Client_connecter* instance;
    Client_connecter() {}
    SOCKET ConnectSocket ;
    int iResult;

public:
    static Client_connecter& getInstance();
    void init_connection(  char *argv);
    void close_socket();
    std::string recieve_message();
    void send_message( std::string message);
    int get_iResult() { return this->iResult; }

};



#endif // CLIENT_CONNECTER_H
