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

    ChessServer server(io_ptr, port);
    server.start_accept();
    std::cin.get();

    return 0;
}