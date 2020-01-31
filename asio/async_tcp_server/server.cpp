#include <utility>
#include <cstdlib>
#include <iostream>

#include <signal.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#ifdef _MSC_VER

std::string strsignal(int sig)
{
    std::stringstream sstream;
    sstream << sig;
    return sstream.str();
}

#endif

using boost::asio::ip::tcp;

boost::function<void()> g_stop_application;

class session : public std::enable_shared_from_this<session>
{
public:
    typedef std::shared_ptr<session>     pointer;
    typedef boost::asio::ip::tcp::socket socket;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new session(io_service));
    }

    socket& get_socket()
    {
        return m_socket;
    }

    void start()
    {
        do_write();
    }

private:
    session(boost::asio::io_service& io_service)
        : m_socket(io_service)
    {
    }

    void do_write()
    {
        auto data = make_message();
        auto self(shared_from_this());
        boost::asio::async_write(m_socket, boost::asio::buffer(data, data.length()),
                                 [this, self] (boost::system::error_code ec, std::size_t length)
        {
            (void)(this);
            if (ec)
            {
                std::cerr << "Worker error: " << ec << std::endl;
            }
        });
    }

    std::string make_message()
    {
        std::stringstream sstream;
        sstream << "worker thread id: " << boost::this_thread::get_id() << "\n";
        return sstream.str();
    }

    tcp::socket m_socket;
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
        : m_io_service(io_service), m_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept()
    {
        session::pointer session = session::create(m_io_service);
        m_acceptor.async_accept(session->get_socket(), [this, session] (boost::system::error_code ec)
        {
            if (!ec)
            {
                session->start();
                do_accept();
            }
        });

    }

    boost::asio::io_service& m_io_service;
    tcp::acceptor m_acceptor;
};

void signal_handler(int signum)
{
    std::cout << "\n" << "catch: " << strsignal(signum) << "\n" << std::endl;
    signal(signum, SIG_DFL);
    g_stop_application();
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: boost_async_tcp_server <port>" << std::endl;
        return 1;
    }
    const int port(std::atoi(argv[1]));

    boost::asio::io_service io_service;
    server s(io_service, port);
    std::cout << "Started server on 127.0.0.1 " << port << std::endl;

    boost::thread_group group;
    for(unsigned i = 0; i < boost::thread::hardware_concurrency(); i++) {
        group.add_thread(new boost::thread([&io_service]() {
            try
            {
                io_service.run();
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
        }));
    }
    g_stop_application = boost::function<void()>([&io_service]() {
        io_service.stop();
    });
    signal(SIGINT, signal_handler);
    group.join_all();

    std::cout << "Server stopped" << std::endl;
    return 0;
}
