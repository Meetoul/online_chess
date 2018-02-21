#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "../engine/asyncplayer.h"
#include "../engine/chessboard.h"
#include "../engine/asyncaiplayer.h"

using namespace boost::asio;

class AsyncClientPlayer : public std::enable_shared_from_this<AsyncClientPlayer>,
                          public AsyncPlayer
{
public:
  AsyncClientPlayer(std::unique_ptr<ip::tcp::socket> socket);

  virtual void asyncPrepare(const ChessBoard &board, ReadyHandler handler) override;
  virtual void asyncGetNext(const ChessBoard &board, MoveReadyHandler handler) override;
  virtual void asyncShowMove(const ChessBoard &board, const Move &move, ReadyHandler handler) override;
  virtual void asyncShowResult(const ChessBoard &board, EndStatus status, ReadyHandler handler) override;

  virtual void cancel() override;

  ip::tcp::socket & socket();

private:
  std::unique_ptr<ip::tcp::socket> m_sock_ptr;
};

typedef std::shared_ptr<AsyncClientPlayer> player_ptr;