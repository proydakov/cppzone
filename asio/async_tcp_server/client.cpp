#include <cstdlib>
#include <cstring>
#include <iostream>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: boost_sync_tcp_client <host> <port>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_service io_service;
        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        boost::asio::connect(s, resolver.resolve({argv[1], argv[2]}));

        char answer[max_length];
        try {
            boost::asio::read(s, boost::asio::buffer(answer, max_length));
        }
        catch(...) {
        }
        std::cout << "Answer is: " << answer;
    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
