#include "server.hpp"
#include <iostream>
#include "../engine/asyncgame.h"

namespace ph = std::placeholders;

ChessServer::ChessServer(std::shared_ptr<io_service> io_ptr, short port)
    : m_io_ptr(io_ptr),
      m_endpoint(ip::tcp::v4(), port),
      m_acceptor(*m_io_ptr, m_endpoint) {}

void ChessServer::handle_accept(const boost::system::error_code &err)
{
    std::cout << "new connection!\n";
    m_players.emplace_back(new AsyncClientPlayer(std::move(m_sock_ptr)));
    m_sock_ptr = std::make_unique<ip::tcp::socket>(*m_io_ptr);
    m_acceptor.async_accept(*m_sock_ptr, std::bind(&ChessServer::handle_accept, this, ph::_1));
    if (m_players.size() == 2)
    {
        start_game();
        return;
    }
}

void ChessServer::start_accept()
{
    m_sock_ptr = std::make_unique<ip::tcp::socket>(*m_io_ptr);
    m_acceptor.async_accept(*m_sock_ptr, std::bind(&ChessServer::handle_accept, this, ph::_1));
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