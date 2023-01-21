
#include <iostream>
#include <WinSock2.h>
#include "ConnectToSQL.h"
#include "PacketManager.h"
#include "ConnectToTCP.h"

using namespace std;

/*
int main(int argc, char* argv[])
{
    ConnectToSQL* mysql = new ConnectToSQL();
    MYSQL_ROW res;
    res = mysql->SQLQuery("select * from objectinfo");
    cout << res[0] << " / " << res[1] << " / " << res[2] << " / " << res[3] << endl;
    
    PacketManager* packetManager = new PacketManager;
    char* outputBuf = packetManager->MakePacket(atoi(res[0]), atoi(res[1]), atoi(res[2]));
    int bufSize = packetManager->GetBufSize();
    //free(outputBuf);

    ConnectToTCP* socket = new ConnectToTCP();
    socket->Connect(8000, "127.0.0.1");
    socket->Send(outputBuf, bufSize);
    free(outputBuf);

    cout << "it's ok" << endl;
    delete mysql;
    delete packetManager; //내부 malloc free시킬 다른 방법 찾을 수 있으면 좋을 듯
    delete socket;
    
    //delete[] outputBuf;
    //outputBuf = NULL;
    
    return 0;
}
*/