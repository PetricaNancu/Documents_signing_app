#include "client_connecter.h"


Client_connecter* Client_connecter::instance = nullptr;
Client_connecter& Client_connecter::getInstance()
{
    if (Client_connecter::instance==nullptr)
    {
        instance = new Client_connecter();
    }
    return *instance;
}

void Client_connecter::init_connection( char * argv)
{
    this->ConnectSocket = INVALID_SOCKET;
    WSAData wsaData;
    struct addrinfo *result = NULL,
        *ptr = NULL,
        hints;

    // Initializare Winsock
    this->iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (this->iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", this->iResult);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Adresa si port pentru SERVER
    this->iResult = getaddrinfo(argv, DEFAULT_PORT, &hints, &result);
    if (this->iResult != 0) {
       // printf("getaddrinfo failed with error: %d\n", this->iResult);
        WSACleanup();
    }

    // Incearca conectarea la adrese
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Creare SOCKET pentru Server
        this->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (this->ConnectSocket == INVALID_SOCKET) {
            //printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
        }

        // Connectare la Server
        this->iResult = connect(this->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (this->iResult == SOCKET_ERROR) {
            closesocket(this->ConnectSocket);
            this->ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (this->ConnectSocket == INVALID_SOCKET)
    {
        //printf("Unable to connect to server!\n");
        WSACleanup();
    }
   // send_message("conectat");
}

void Client_connecter::close_socket()
{
    this->iResult = shutdown(this->ConnectSocket, SD_SEND);
    if (this->iResult == SOCKET_ERROR) {
       // printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(this->ConnectSocket);
        WSACleanup();

    }

    // cleanup
    closesocket(this->ConnectSocket);
    WSACleanup();
}

std::string Client_connecter::recieve_message()
{
    char buf[DEFAULT_BUFLEN];
    this->iResult = recv(this->ConnectSocket, buf, DEFAULT_BUFLEN, 0);
    buf[this->iResult] = '\0';

    return buf;
}

void Client_connecter::send_message( std::string message)
{
    this->iResult = send(this->ConnectSocket, message.c_str(), (int)message.length(), 0);
  //  std::cout << "Am trimis :" << message << std::endl;

    if (this->iResult == SOCKET_ERROR) {
        //printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->ConnectSocket);
        WSACleanup();
    }
}

