#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

class WSASession
{
public:
	WSASession()
	{
		int ret = WSAStartup(MAKEWORD(2, 2), &data);
		if (ret != 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
	}
	~WSASession()
	{
		WSACleanup();
	}
private:
	WSAData data;
};

class UDPSocket
{
public:
	UDPSocket()
	{
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock == INVALID_SOCKET)
			throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
	}
	~UDPSocket()
	{
		closesocket(sock);
	}

	void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0)
	{
		sockaddr_in add;
		add.sin_family = AF_INET;
		add.sin_addr.s_addr = inet_addr(address.c_str());
		add.sin_port = htons(port);
		int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
	}
	void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0)
	{
		int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&address), sizeof(address));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
	}
	sockaddr_in RecvFrom(char* buffer, int len, int flags = 0)
	{
		sockaddr_in from;
		int size = sizeof(from);
		int ret = recvfrom(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&from), &size);
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");

		// make the buffer zero terminated
		buffer[ret] = 0;
		return from;
	}
	void Bind(unsigned short port)
	{
		sockaddr_in add;
		add.sin_family = AF_INET;
		add.sin_addr.s_addr = htonl(INADDR_ANY);
		add.sin_port = htons(port);

		int ret = bind(sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
		if (ret < 0)
			throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
	}

private:
	SOCKET sock;
};

int main()
{
	
	std::thread server([] {
		try
		{
			WSASession Session;
			UDPSocket Socket;
			char buffer[100];

			Socket.Bind(100);
			while (1)
			{
				sockaddr_in add = Socket.RecvFrom(buffer, sizeof(buffer));

				std::string input(buffer);
				std::cout << "server:" << buffer << std::endl;
				std::reverse(std::begin(input), std::end(input));
				Socket.SendTo(add, input.c_str(), input.size());
				Sleep(1);
			}
		}
		catch (std::system_error& e)
		{
			std::cout << e.what();
		}
	});

	server.detach();


	std::thread client([] {
	
			try
			{
				WSASession Session;
				UDPSocket Socket;
				std::string data = "hello world";
				char buffer[100];
				while (1) {
					Socket.SendTo("127.0.0.1", 100, data.c_str(), data.size());
				}
				Socket.RecvFrom(buffer, 100);
				std::cout << "client:" << buffer << std::endl;
			}
		
			catch (std::exception &ex)
			{
				std::cout << ex.what();
			}
			char c;
			std::cin >> c;
			Sleep(1);
		
	});
	client.detach();
	system("pause");
}
