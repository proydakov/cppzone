#include <chrono>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

template<class T>
void check_error(T error)
{
    if(error) {
        std::stringstream sstream;
        sstream << error;
        throw std::runtime_error(sstream.str());
    }
}

class client : public std::enable_shared_from_this<client>
{
public:
    client(boost::asio::io_service& io_service) :
        socket_(io_service)
    {
        std::clog << "<- client" << std::endl;
    }

    ~client()
    {
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

        size_t elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start_time_).count();

        size_t bit_in_sec = accumulator_ / elapsed_seconds * 8;

        double bandwidth = bit_in_sec;
        std::string units = "Bit/s";
        if(bit_in_sec > 1000000000) {
            bandwidth = bit_in_sec / 1000000000.0;
            units = "GBit/s";
        }
        else if(bit_in_sec > 1000000) {
            bandwidth = bit_in_sec / 1000000.0;
            units = "MBit/s";
        }
        else if(bit_in_sec > 1000) {
            bandwidth = bit_in_sec / 1000.0;
            units = "KBit/s";
        }

        std::clog << "<- ~client. bytes received: " << accumulator_
                  << " process time: " << elapsed_seconds << " (seconds)"
                  << " bandwidth: " << bandwidth << " " << units
                  << std::endl;
    }

    void go(const std::string& server, int port)
    {
        std::clog << "<- go" << std::endl;

        auto self(shared_from_this());

        boost::system::error_code err;

        tcp::endpoint endpoint( boost::asio::ip::address::from_string(server), port );

        std::clog << "<- schedule async_connect" << std::endl;
        socket_.async_connect(endpoint,
            [this, self](boost::system::error_code ec) {
                if (!ec) {
                    do_write();
                }
        });
    }

private:
    void do_write()
    {
        start_time_ = std::chrono::system_clock::now();

        auto self(shared_from_this());
        build_request();
        boost::asio::async_write(socket_, request_,
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    do_read();
                }
        });
    }

    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(response_buffer_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                accumulator_ += length;
                if (!ec) {
                    do_read();
                }
        });
    }

    void build_request()
    {
        std::ostream request_stream(&request_);
        request_stream << "TEST";
    }

private:
    tcp::socket socket_;
    boost::asio::streambuf request_;

    std::chrono::time_point<std::chrono::system_clock> start_time_;

    enum { max_length = 1024 * 8 };
    std::array<char, max_length> response_buffer_;

    size_t accumulator_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3) {
            std::cout << "Usage: async_speed_client <server> <port>\n";
            return 1;
        }

        //std::clog.setstate(std::ios_base::failbit);

        boost::asio::io_service io_service;
        auto c = std::make_shared<client>(io_service);
        c->go( argv[1], std::stoi(argv[2]) );
        c.reset();

        std::clog << "<- io_service run" << std::endl;

        io_service.run();

        std::clog << "<- io_service done" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "<- main exception: " << e.what() << "\n";
    }

    return 0;
}
