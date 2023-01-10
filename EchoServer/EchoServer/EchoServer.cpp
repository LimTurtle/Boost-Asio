// EchoServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <boost/asio.hpp>
#include <array>


using namespace boost::asio;
using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
    io_service io_service;
    ip::tcp::endpoint endpoint(ip::tcp::v4(), PORT_NUMBER);
    ip::tcp::acceptor acceptor(io_service, endpoint);

    ip::tcp::socket socket(io_service);
    acceptor.accept(socket);
    cout << "클라이언트 접속" << endl;

    for (;;)
    {
        char buf[128] = { 0, };
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error)
        {
            if(error == boost::asio::error::eof)
            {
                cout << "클라이언트와 연결이 끊어졌습니다." << endl;
            }
            else
            {
                cout << "error No: " << error.value() << " error Message: " << error.message() << endl;
            }
            break;
        }

        cout << "클라이언트에서 받은 메시지: " << &buf[0] << endl;

        char szMessage[128] = { 0, };
        sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
        int nMsgLen = strnlen_s(szMessage, 128 - 1);
        boost::system::error_code ignore_error;
        socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignore_error);
        cout << "클라이언트에 보낸 메시지: " << szMessage << endl;
    }
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
