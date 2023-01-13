# Boost/asio & Unity TCP 비동기 통신

## Boost/asio Server
- 비동기 통신 시 ```async_XXX``` 키워드를 사용해야 하고, bind를 통해 해당 작업이 끝난 뒤 호출할 함수를 정의해야 한다.
```cpp
#include <boost/asio.hpp>
#include <boost/bind.hpp>

class Session
{
public:
    Session(boost::asio::io_context& io_service) : m_Socket(io_service) {} //io_service는 지원이 중단 됨. 따라서 io_context 사용

    boost::asio::ip::tcp::socket& Socket()
    {
        return m_Socket;
    }

    void PostReceive()
    {
        memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer)); //Buffer 초기화
        m_Socket.async_read_some //read_some의 비동기화
        (
            boost::asio::buffer(m_ReceiveBuffer), //읽은 데이터를 저장할 Buffer
            boost::bind(&Session::handle_receive, this, //작업이 끝난 뒤 호출할 함수
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)
            );
    }

private:
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred) {}
    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
    {
           const string strRecvMessage = m_ReceiveBuffer.data(); //.data() 통해 Buffer 내용을 string형으로 변환
           char szMessage[128] = { 0, };
           sprintf_s(szMessage, 128 - 1, "전송할 문자열"); //이 때 문자열은 char * 형태로 보내야 하므로, .c_str() 메소드 활용
           m_WriteMessage = szMessage;
           boost::asio::async_write //write의 비동기화
           (m_Socket, //write 시 Socket 내용을 추가
               boost::asio::buffer(m_WriteMessage), //전송할 Buffer
               boost::bind(&Session::handle_write, this, //작업이 끝난 뒤 호출할 함수
               boost::asio::placeholders::error,
               boost::asio::placeholders::bytes_transferred)
               );
           PostReceive();
    }

    boost::asio::ip::tcp::socket m_Socket;
};

class TCP_Server
{
public:
    TCP_Server(boost::asio::io_context& io_service) : m_acceptor(io_service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)) //동기 통신의 accept와 동일
    {
        m_pSession = nullptr;
        StartAccept();
    }

    ~TCP_Server()
    {
        if (m_pSession != nullptr)
        {
            delete m_pSession;
        }
    }

private:
    void StartAccept()
    {
        m_pSession = new Session((boost::asio::io_context&)m_acceptor.get_executor().context()); //.get_io_service() 지원이 중단되어, context() 사용
        m_acceptor.async_accept //accept의 비동기화
        (m_pSession->Socket(), //Session의 Socket 사용
            boost::bind(&TCP_Server::handle_accept, this, m_pSession, //accept 작업이 끝났을 때 호출할 함수
            boost::asio::placeholders::error)
        );
    }

    void handle_accept(Session* pSession, const boost::system::error_code& error)
    {
        if (!error)
        {
            pSession->PostReceive();
        }
    }

    int m_nSeqNumber;
    boost::asio::ip::tcp::acceptor m_acceptor;
    Session* m_pSession;
};
```

## Unity Client
- ContextMenu 메소드 사용해 SendMessage 함수를 사용자가 필요할 때 실행할 수 있도록 함.
- Thread를 이용해 서버로부터 Read를 실시간으로 받아올 수 있도록 구현
- [참고 자료](https://gist.github.com/danielbierwirth/0636650b005834204cb19ef5ae6ccedb)

# Boost/asio Multi Client - Server간 통신
- 작성 중
