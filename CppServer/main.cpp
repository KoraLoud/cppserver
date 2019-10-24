#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "3000"

int main()
{
	//init winsock

	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int errresult = WSAStartup(version, &data);
	if (errresult != 0)
	{
		std::cerr << "init failed code: " << errresult << std::endl;
		return 1;
	}

	//create server socket

	struct addrinfo* result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	errresult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (errresult != 0)
	{
		std::cerr << "create socket failed code: " << errresult << std::endl;
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//bind socket

	errresult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (errresult == SOCKET_ERROR)
	{
		std::cerr << "bind failed with err: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	
	freeaddrinfo(result);
	
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "listen failed with err: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//client socket
	//TODO: expand for multiple clients
	//https://docs.microsoft.com/en-us/windows/win32/winsock/accepting-a-connection

	SOCKET ClientSocket;


	return 0;
}