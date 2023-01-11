// EchoClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

class TCP_Client
{
public:
    TCP_Client(boost::asio::io_context& io_service) :
        m_io_service(io_service), m_Socket(io_service), m_nSeqNumber(0) {}

    void Connect(boost::asio::ip::tcp::endpoint& endpoint)
    {
        m_Socket.async_connect(endpoint, boost::bind(&TCP_Client::handle_connet, this, boost::asio::placeholders::error)
        );
    }

private:
    void PostWrite()
    {
        if (m_Socket.is_open() == false)
        {
            return;
        }

        if (m_nSeqNumber > 7)
        {
            m_Socket.close();
            return;
        }

        m_nSeqNumber += 1;

        char szMessage[128] = { 0, };
        sprintf_s(szMessage, 128 - 1, "%d - Send Message", m_nSeqNumber);
        m_WriteMessage = szMessage;

        boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
            boost::bind(&TCP_Client::handle_write, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)
        );

        PostReceive();
    }

    void PostReceive()
    {
        memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));
        m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
            boost::bind(&TCP_Client::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)
        );
    }

    void handle_connet(const boost::system::error_code& error)
    {
        if (error)
        {
            cout << "connect failed : " << error.message() << endl;
        }
        else
        {
            cout << "connected" << endl;
            PostWrite();
        }
    }

    void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
    {

    }
    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
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
            const string strRecvMessage = m_ReceiveBuffer.data();
            cout << "서버에서 받은 메시지: " << strRecvMessage << ", 받은 크기: " << bytes_transferred << endl;
            PostWrite();
        }
    }

    boost::asio::io_context& m_io_service;
    boost::asio::ip::tcp::socket m_Socket;
    int m_nSeqNumber;
    array<char, 128> m_ReceiveBuffer;
    string m_WriteMessage;
};

int main()
{
    boost::asio::io_context io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);
    TCP_Client client(io_service);
    client.Connect(endpoint);
    io_service.run();
    cout << "네트워크 접속 종료" << endl;
    getchar();
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
