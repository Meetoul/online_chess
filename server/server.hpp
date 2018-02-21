#pragma once

#include <boost/asio.hpp>
#include <vector>

#include "asyncclientplayer.hpp"

using namespace boost::asio;

class ChessServer
{
public:
  ChessServer(std::shared_ptr<io_service> io_ptr, short port);
  void start_accept();

private:
  std::shared_ptr<io_service> m_io_ptr;

  ip::tcp::endpoint m_endpoint;
  ip::tcp::acceptor m_acceptor;
  std::unique_ptr<ip::tcp::socket> m_sock_ptr;

  std::vector<player_ptr> m_players;

  void handle_accept(const boost::system::error_code &err);
  void start_game();
};