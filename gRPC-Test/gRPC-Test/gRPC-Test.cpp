﻿// gRPC-Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>
#include "CProtocol.pb.h"

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
    Protocol::S_DATA data;
    data.set_id(1);
    data.set_map_level(10);
    data.set_match_room(1);

    int bufSize = 0;
    bufSize += MessageHeaderSize + data.ByteSizeLong();
    protobuf::uint8* outputBuf = new protobuf::uint8[bufSize];

    protobuf::io::ArrayOutputStream output_array_stream(outputBuf, bufSize);
    protobuf::io::CodedOutputStream output_coded_stream(&output_array_stream);

    WriteMessageToStream(Protocol::MOVE, data, output_coded_stream);

    protobuf::io::ArrayInputStream input_array_stream(outputBuf, bufSize);
    protobuf::io::CodedInputStream input_coded_stream(&input_array_stream);

    PacketProcess(input_coded_stream);

    delete[] outputBuf;
    outputBuf = NULL;
    return 0;
}
