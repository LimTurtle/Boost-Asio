#include "BoostAsio.h"

BoostAsio::~BoostAsio()
{
    delete mysql;
    delete packetManager; //내부 malloc free시킬 다른 방법 찾을 수 있으면 좋을 듯
}

void BoostAsio::Connect(boost::asio::ip::tcp::endpoint& endpoint)
{
	m_Socket.async_connect(endpoint, boost::bind(&BoostAsio::handle_connect, this, boost::asio::placeholders::error)
	);
}

void BoostAsio::PostWrite()
{
    if (m_Socket.is_open() == false)
    {
        return;
    }

    /*
    if (m_nSeqNumber > 7)
    {
        m_Socket.close();
        return;
    }
    */
    

    m_nSeqNumber += 1;
    //char* outputBuf;
    if (m_nSeqNumber == 1)
    {
        outputBuf = packetManager->MakePacket(atoi(res[0]), atoi(res[1]), atoi(res[2]));
        temp[0] = atoi(res[0]);
        temp[1] = atoi(res[1]);
        temp[2] = atoi(res[2]);
        //temp[3] = atoi(res[3]);
    }
    else
    {
        temp[0] += 1;
        outputBuf = packetManager->MakePacket(temp[0], temp[1], temp[2]);
    }
    
    bufSize = packetManager->GetBufSize();
    //string str = outputBuf;
    //char szMessage[128] = { 0, };
    //sprintf_s(szMessage, 128 - 1, "%d - Send Message", m_nSeqNumber);
    m_WriteMessage = outputBuf;

    boost::asio::async_write(m_Socket, boost::asio::buffer(outputBuf, bufSize),
        boost::bind(&BoostAsio::handle_write, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
    );

    PostReceive();
}

void BoostAsio::PostReceive()
{
    //memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

    m_Socket.async_read_some(boost::asio::buffer(outputBuf, bufSize),
        boost::bind(&BoostAsio::handle_receive, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
    );
}

void BoostAsio::handle_connect(const boost::system::error_code& error)
{
    if (error)
    {
        cout << "connect failed : " << error.message() << endl;
    }
    else
    {
        cout << "connected" << endl;

        mysql = new ConnectToSQL();
        res = mysql->SQLQuery("select * from objectinfo");
        cout << res[0] << " / " << res[1] << " / " << res[2] << " / " << res[3] << endl;
        packetManager = new PacketManager;

        PostWrite();
    }
}

void BoostAsio::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (error)
    {
        if (error == boost::asio::error::eof)
        {
            cout << "서버와 연결이 끊어졌습니다." << endl;
        }
        else
        {
            cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
        }
    }
    else
    {
        cout << m_nSeqNumber << " 전송 완료" << endl;
        //PostWrite();
    }
}

void BoostAsio::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (error)
    {
        if (error == boost::asio::error::eof)
        {
            cout << "서버와 연결이 끊어졌습니다." << endl;
        }
        else
        {
            cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
        }
    }
    else
    {
        protobuf::io::ArrayInputStream input_array_stream(outputBuf, bufSize);
        protobuf::io::CodedInputStream input_coded_stream(&input_array_stream);
        packetManager->PacketProcess(input_coded_stream);
        const string strRecvMessage = m_ReceiveBuffer.data();
        //cout << "서버에서 받은 메시지: " << strRecvMessage << ", 받은 크기: " << bytes_transferred << endl;
        PostWrite();
    }
}

int main(int argc, char* argv[])
{
    boost::asio::io_context io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8000);
    BoostAsio client(io_service);
    client.Connect(endpoint);
    io_service.run();
    cout << "네트워크 접속 종료" << endl;
    getchar();
    return 0;
}