#include "server.hpp"
#include <iostream>
#include "../engine/asyncgame.h"

namespace ph = std::placeholders;

ChessServer::ChessServer(std::shared_ptr<io_service> io_ptr, short port)
    : m_io_ptr(io_ptr),
      m_endpoint(ip::tcp::v4(), port),
      m_acceptor(*m_io_ptr, m_endpoint) {}

void ChessServer::handle_accept(player_ptr player, const boost::system::error_code &err)
{
    std::cout << "new connection!\n";
    m_players.emplace_back(player);
    if (m_players.size() == 2)
    {
        start_game();
        return;
    }
    auto new_player = std::make_shared<AsyncClientPlayer>(m_io_ptr);
    m_acceptor.async_accept(new_player->socket(), std::bind(&ChessServer::handle_accept, this, new_player, ph::_1));
}

void ChessServer::start_accept()
{
    auto player = std::make_shared<AsyncClientPlayer>(m_io_ptr);
    m_acceptor.async_accept(player->socket(), std::bind(&ChessServer::handle_accept, this, player, ph::_1));
    m_io_ptr->run();
}

void ChessServer::start_game()
{
    m_players[0]->setColor(WHITE);
    char color[] = {0};
    m_players[0]->socket().write_some(buffer(color));
    m_players[1]->setColor(BLACK);
    color[0] = 16;
    m_players[1]->socket().write_some(buffer(color));
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