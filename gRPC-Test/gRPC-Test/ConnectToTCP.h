#pragma once
#include <iostream>
#include <WinSock2.h>
#include <string>

using namespace std;

class ConnectToTCP
{
public:
	ConnectToTCP();
	~ConnectToTCP();
	void Connect(int port, string ip);
	void Send(char* outputBuf, int bufSize);
private:
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN tAddr = {};
};