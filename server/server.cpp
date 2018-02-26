#include <iostream>
#include <ctime>

#include "server.hpp"
#include "../engine/asyncgame.h"

namespace ph = std::placeholders;

ChessServer::ChessServer(std::shared_ptr<io_service> io_ptr, short port)
    : m_io_ptr(io_ptr),
      m_endpoint(ip::tcp::v4(), port),
      m_acceptor(*m_io_ptr, m_endpoint) {}

void ChessServer::handle_accept(const boost::system::error_code &err)
{
    std::cout << "new connection!\n";
    m_players.emplace_back(new AsyncOnlinePlayer(std::move(m_sock_ptr)));
    m_sock_ptr = std::make_unique<ip::tcp::socket>(*m_io_ptr);
    m_acceptor.async_accept(*m_sock_ptr, std::bind(&ChessServer::handle_accept, this, ph::_1));
    if (m_players.size() == 2)
    {
        pre_game();
        return;
    }
}

void ChessServer::start_accept()
{
    m_sock_ptr = std::make_unique<ip::tcp::socket>(*m_io_ptr);
    m_acceptor.async_accept(*m_sock_ptr, std::bind(&ChessServer::handle_accept, this, ph::_1));
}

void ChessServer::pre_game()
{
    srand(0);
    int color = rand() % 2 ? WHITE : BLACK;
    m_players[0]->setColor(color);
    m_players[1]->setColor(TOGGLE_COLOR(color));
    char first_color[1] = {color};
    char second_color[1] = {TOGGLE_COLOR(color)};
    m_players[0]->socket().write_some(buffer(first_color));
    m_players[1]->socket().write_some(buffer(second_color));
    start_game();
}

void ChessServer::start_game()
{
    AsyncGame game(m_io_ptr, m_players[0], m_players[1]);
    game.start([this](AsyncPlayer::EndStatus end_status) {
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
        m_io_ptr->stop();
    });
}