// gRPC-Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>
#include "CProtocol.pb.h"
#include <WinSock2.h>
#include "ConnectToSQL.h"

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;
using namespace google;

struct MessageHeader
{
    protobuf::uint32 size;
    Protocol::INGAME type;
};
const int MessageHeaderSize = sizeof(MessageHeader);


void PrintMsg(::google::protobuf::Message& msg)
{
    string str;
    str = msg.DebugString();
    cout << str << endl;
}

void PacketProcess(protobuf::io::CodedInputStream& input_stream)
{
    MessageHeader msgHeader;
    while (input_stream.ReadRaw(&msgHeader, MessageHeaderSize))
    {
        const void* payload_ptr = NULL;
        int remainSize = 0;
        input_stream.GetDirectBufferPointer(&payload_ptr, &remainSize);
        if (remainSize < (signed)msgHeader.size) break;

        protobuf::io::ArrayInputStream payload_array_stream(payload_ptr, msgHeader.size);
        protobuf::io::CodedInputStream payload_input_stream(&payload_array_stream);
        input_stream.Skip(msgHeader.size);

        switch (msgHeader.type)
        {
        case Protocol::MOVE:
        {
            Protocol::S_DATA packet;
            if (packet.ParseFromCodedStream(&payload_input_stream) == false) break;
            PrintMsg(packet);
            break;
        }

        default:
            cout << "Wrong Packet" << endl;
            break;
        }
    }
}

    


void WriteMessageToStream(Protocol::INGAME msgType, const protobuf::Message& message,
    protobuf::io::CodedOutputStream& stream)
{
    MessageHeader msgHeader;
    msgHeader.size = message.ByteSizeLong();
    msgHeader.type = msgType;
    stream.WriteRaw(&msgHeader, sizeof(MessageHeader));
    message.SerializeToCodedStream(&stream);
}

int main(int argc, char* argv[])
{
    ConnectToSQL* mysql = new ConnectToSQL();
    MYSQL_ROW res;
    res = mysql->SQLQuery("select * from objectinfo");
    cout << res[0] << " / " << res[1] << " / " << res[2] << " / " << res[3] << endl;
    
    Protocol::S_DATA data;
    data.set_id(atoi(res[0]));
    data.set_map_level(atoi(res[1]));
    data.set_match_room(atoi(res[2]));

    int bufSize = 0;
    bufSize += MessageHeaderSize + data.ByteSizeLong();
    //protobuf::uint8* outputBuf = new protobuf::uint8[bufSize];
    char* outputBuf = (char*)malloc(bufSize * sizeof(char));

    protobuf::io::ArrayOutputStream output_array_stream(outputBuf, bufSize);
    
    protobuf::io::CodedOutputStream output_coded_stream(&output_array_stream);

    WriteMessageToStream(Protocol::MOVE, data, output_coded_stream);


    
    protobuf::io::ArrayInputStream input_array_stream(outputBuf, bufSize);
    protobuf::io::CodedInputStream input_coded_stream(&input_array_stream);

    PacketProcess(input_coded_stream);

    

    
    
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
    delete[] outputBuf;
    outputBuf = NULL;
    
    return 0;
}
