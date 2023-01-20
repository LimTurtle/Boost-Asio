
#include <iostream>
#include <WinSock2.h>
#include "ConnectToSQL.h"
#include "PacketManager.h"

using namespace std;

int main(int argc, char* argv[])
{
    ConnectToSQL* mysql = new ConnectToSQL();
    MYSQL_ROW res;
    res = mysql->SQLQuery("select * from objectinfo");
    cout << res[0] << " / " << res[1] << " / " << res[2] << " / " << res[3] << endl;
    
    PacketManager* packetManager = new PacketManager;
    char* outputBuf = packetManager->MakePacket(atoi(res[0]), atoi(res[1]), atoi(res[2]));
    int bufSize = packetManager->GetBufSize();
    free(packetManager); //내부 malloc free시킬 다른 방법 찾을 수 있으면 좋을 듯

    try {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        SOCKET hSocket;
        hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        SOCKADDR_IN tAddr = {};
        tAddr.sin_family = AF_INET;
        tAddr.sin_port = htons(8000);
        tAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

        char cBuf[] = "Client Send";
        send(hSocket, outputBuf, bufSize, 0);
        closesocket(hSocket);
        WSACleanup();
        
    }
    catch (exception e)
    {
        cout << e.what() << endl;
    }

    cout << "it's ok" << endl;
    delete mysql;
    
    //delete[] outputBuf;
    //outputBuf = NULL;
    
    return 0;
}
