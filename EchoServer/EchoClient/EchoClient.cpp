// EchoClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <boost/asio.hpp>

using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);

    boost::system::error_code connect_error;
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(endpoint, connect_error);

    if (connect_error)
    {
        cout << "연결 실패. error No: " << connect_error.value() << ", Message: " << connect_error.message() << endl;
        getchar();
        return 0;
    }
    else {
        cout << "서버에 연결 성공" << endl;
    }

    for (int i = 0; i < 7; i++) {
        char szMessage[128] = { 0, };
        sprintf_s(szMessage, 128 - 1, "%d - Send Message", i);
        int nMsgLen = strnlen_s(szMessage, 128 - 1);

        boost::system::error_code ignored_error;
        socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);
        cout << "서버에 보낸 메시지: " << szMessage << endl;

        char buf[128] = { 0, };
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error)
        {
            if (error == boost::asio::error::eof)
            {
                cout << "서버와 연결이 끊어졌습니다." << endl;
            }
            else
            {
                cout << "error No: " << error.value() << ", Message: " << error.message().c_str() << endl;
            }
            break;
        }
        cout << "서버로부터 받은 메시지: " << &buf[0] << endl;
    }

    if (socket.is_open())
    {
        socket.close();
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
