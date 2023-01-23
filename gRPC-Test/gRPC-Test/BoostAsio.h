#pragma once
// EchoClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "ConnectToSQL.h"
#include "PacketManager.h"

using namespace std;

class BoostAsio
{
public:
    BoostAsio(boost::asio::io_context& io_service) :
        m_io_service(io_service), m_Socket(io_service), m_nSeqNumber(0) {}
    ~BoostAsio();

    void Connect(boost::asio::ip::tcp::endpoint& endpoint);
private:
    void PostWrite();

    void PostReceive();

    void handle_connect(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context& m_io_service;
    boost::asio::ip::tcp::socket m_Socket;
    int m_nSeqNumber;
    char* outputBuf;
    int bufSize;
    array<char, 128> m_ReceiveBuffer;
    array<float, 3> temp;
    string m_WriteMessage;

    ConnectToSQL* mysql;
    MYSQL_ROW res;

    PacketManager* packetManager;
};
