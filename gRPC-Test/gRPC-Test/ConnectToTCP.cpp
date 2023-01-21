#include "ConnectToTCP.h"

ConnectToTCP::ConnectToTCP()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ConnectToTCP::~ConnectToTCP()
{
	closesocket(hSocket);
	WSACleanup();
}

void ConnectToTCP::Connect(int port, string ip)
{
	try
	{
		tAddr.sin_family = AF_INET;
		tAddr.sin_port = htons(port);
		tAddr.sin_addr.s_addr = inet_addr(ip.c_str());
		connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}

void ConnectToTCP::Send(char* outputBuf, int bufSize)
{
	try
	{
		send(hSocket, outputBuf, bufSize, 0);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}
