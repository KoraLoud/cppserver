#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "3000"
#define DEFAULT_BUFLEN 512

/*
* Written By Kora Loudermilk
*/

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

	struct addrinfo* result = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	errresult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
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

	fd_set connections;
	FD_ZERO(&connections);
	FD_SET(ListenSocket, &connections); //init listen socket for first entry in fd_set

	while (true)
	{
		fd_set setCopy = connections;

		int connectionCount = select(0, &setCopy, nullptr, nullptr, nullptr);

		for (int i = 0; i < connectionCount; i++)
		{
			SOCKET socket = setCopy.fd_array[i];
			if (socket == ListenSocket)  //accept connections
			{
				SOCKET client = accept(ListenSocket, nullptr, nullptr);
				FD_SET(client, &connections);
				//std::string connected = "connection successful";
				//send(client, connected.c_str(), connected.size() + 1, 0);
				std::cout << "client connected" << std::endl;
			}
			else
			{
				char buf[DEFAULT_BUFLEN];
				ZeroMemory(buf, DEFAULT_BUFLEN);

				int bytesRecv = recv(socket, buf, DEFAULT_BUFLEN, 0);
				if (bytesRecv <= 0)
				{
					closesocket(socket);
					FD_CLR(socket, &connections);
				}
				else
				{
					for (int i = 0; i < connections.fd_count; i++)
					{
						SOCKET outSocket = connections.fd_array[i];
						if (outSocket != ListenSocket)
						{
							std::cout << "data recieved : " << std::string(&buf[0], bytesRecv) << std::endl;
							send(outSocket, buf, bytesRecv, 0);
						}
					}
				}
			}
		}

	}
}