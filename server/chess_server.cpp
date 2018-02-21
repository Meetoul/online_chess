#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "server.hpp"

using namespace boost::asio;

int parse_arguments(int argv, char **argc, unsigned short &port)
{
    if (argv == 2)
    {
        port = std::stoi(argc[1]);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    unsigned short port;
    if (parse_arguments(argc, argv, port))
    {
        std::cout << "Usage is: ./chess_server [port]\n";
        return 0;
    }

    auto io_ptr = std::make_shared<io_service>();
    signal_set signals(*io_ptr, SIGINT);
    signals.async_wait([](const boost::system::error_code &err, int sig_number) {
        std::cout << "Server stopped!";
        exit(0);
    });

    ChessServer server(io_ptr, port);
    server.start_accept();

    std::cout << "Server running on port " << port << ".\nPress Ctrl+C to stop server.\n";

    io_ptr->run();

    return 0;
}