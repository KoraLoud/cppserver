#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "3000"
#define DEFAULT_BUFLEN 512

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
	


	char str[INET_ADDRSTRLEN];

	//inet_ntop(result->ai_family, result->ai_addr, str, INET_ADDRSTRLEN);

	//std::cout << "on addr " << str << " on port " << DEFAULT_PORT << std::endl;
	std::cout << "bound to port " << DEFAULT_PORT << " at address 127.0.0.1"<< std::endl;

	freeaddrinfo(result);
	
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "listen failed with err: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	//client socket
	//TODO: expand for multiple clients

	SOCKET ClientSocket;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cerr << "invalid socket " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "connectd" << std::endl;
	char recvbuf[DEFAULT_BUFLEN];
	int recvResult, sendResult;
	int recvbuflen = DEFAULT_BUFLEN;


	//loop until connection is ended
	do 
	{
		recvResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (recvResult > 0)
		{
			std::string recmessage = std::string(&recvbuf[0], recvResult);
			std::cout << recmessage;
			sendResult = send(ClientSocket, recvbuf, recvResult, 0);
			if (sendResult == SOCKET_ERROR)
			{
				std::cerr << "send failed: " << WSAGetLastError() << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (recvResult == 0)
		{
			std::cout << "connection closing" << std::endl;
		}
		else {
			std::cout << "recv failed: " << WSAGetLastError() << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (recvResult > 0);

	errresult = shutdown(ClientSocket, SD_SEND);
	if (errresult == SOCKET_ERROR)
	{
		std::cerr << "shutdown failed " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}