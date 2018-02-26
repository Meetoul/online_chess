#include <iostream>
#include <string>
#include "../engine/asyncgame.h"
#include "../engine/asynconlineplayer.h"
#include "../engine/asyncaiplayer.h"
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;

int parse_arguments(int argv, char **argc, std::string client_name, std::string &ip, unsigned short &port)
{
    if (argv == 4)
    {
        client_name = argc[1];
        ip = argc[2];
        port = std::stoi(argc[3]);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    std::string client_name;
    std::string ip;
    unsigned short port;
    if (parse_arguments(argc, argv, client_name, ip, port))
    {
        std::cout << "Usage is: ./chess_client [client_name] [ip (v4)] [port]\n";
        return 0;
    }

    auto io_ptr = std::make_shared<io_service>();
    auto socket_ptr = std::make_unique<ip::tcp::socket>(*io_ptr);

    ip::tcp::endpoint ep(ip::address::from_string(ip), port);
    socket_ptr->connect(ep);

    char color_buffer[1];
    socket_ptr->read_some(buffer(color_buffer));
    auto ai_player = std::make_shared<AsyncAiPlayer>(1);
    auto server_player = std::make_shared<AsyncOnlinePlayer>(std::move(socket_ptr));
    ai_player->setColor(color_buffer[0]);
    server_player->setColor(TOGGLE_COLOR(color_buffer[0]));

    std::cout << "Ai color is " << COLOR_STR(ai_player->getColor())
              << " and server color is " << COLOR_STR(server_player->getColor()) << std::endl;

    AsyncGame game(io_ptr, ai_player, server_player);
    game.start([io_ptr](AsyncPlayer::EndStatus end_status) {
        switch (end_status)
        {
        case AsyncPlayer::WHITE_WIN:
            std::cout << "White win" << std::endl;
            break;
        case AsyncPlayer::DRAW:
            std::cout << "Draw" << std::endl;
            break;
        case AsyncPlayer::WHITE_LOOSE:
            std::cout << "White loose" << std::endl;
            break;
        }
        io_ptr->stop();
    });

    io_ptr->run();

    return 0;
}