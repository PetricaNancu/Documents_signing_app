#include "Server_connecter.h"

Server_connecter* Server_connecter::instance = nullptr;
Server_connecter& Server_connecter::getInstance() 
{
	if (Server_connecter::instance == nullptr)
	{
		instance = new Server_connecter();
	}
	return *instance;
}

void Server_connecter::initialize_socket()
{
	this->ClientSocket = INVALID_SOCKET;
	this->ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	WSADATA wsaData;

	// Initializare Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
		ExceptionsHandler e("WSAStartup failed with error: " + iResult);
		

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Adresa si port pentru SERVER

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		ExceptionsHandler e("getaddrinfo failed with error: " + iResult);
		WSACleanup();
	}

	// Creare SOCKET
	this->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->ListenSocket == INVALID_SOCKET) {
		ExceptionsHandler e("socket failed with error: " + WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	iResult = bind(this->ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		ExceptionsHandler e("bind failed with error: " + WSAGetLastError());
		freeaddrinfo(result);
		closesocket(this->ListenSocket);
		WSACleanup();
	}

	freeaddrinfo(result);

	iResult = listen(this->ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		ExceptionsHandler e("listen failed with error: " + WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
	}
}

void Server_connecter::accept_socket()
{
	//conectarea celor 2 Socket-uri

	this->ClientSocket = accept(ListenSocket, NULL, NULL);
	if (this->ClientSocket == INVALID_SOCKET) {
		ExceptionsHandler e("accept failed with error: " + WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
}

void Server_connecter::close_socket()
{
	closesocket(ListenSocket);

	// inchidem conexiunea
	int iResult = shutdown(this->ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		ExceptionsHandler e("shutdown failed with error: " + WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}

	// cleanup
	closesocket(this->ClientSocket);
	WSACleanup();
}

void Server_connecter::send_message(std::string message)
{
	iResult = send(this->ClientSocket, message.c_str(), (int)message.length(), 0);
	if (message != "ok")
		std::cout << "Am trimis :" << message << std::endl;


	if (iResult == SOCKET_ERROR) {
		ExceptionsHandler e("send failed with error: " + WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}
}
void Server_connecter::send_file(std::string message)
{
	iResult = send(this->ClientSocket, message.c_str(), (int)message.length(), 0);

	if (iResult == SOCKET_ERROR) {
		ExceptionsHandler e("send failed with error: " + WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}
}

std::string Server_connecter::recieve_message()
{
	char buf[DEFAULT_BUFLEN];

	iResult = recv(this->ClientSocket, buf, DEFAULT_BUFLEN, 0);
	buf[iResult] = '\0';
	return buf;

}


