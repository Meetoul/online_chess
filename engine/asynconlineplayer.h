#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "asyncplayer.h"
#include "chessboard.h"

using namespace boost::asio;

class AsyncOnlinePlayer : public std::enable_shared_from_this<AsyncOnlinePlayer>,
                          public AsyncPlayer
{
  public:
    AsyncOnlinePlayer(std::unique_ptr<ip::tcp::socket> socket);

    virtual void asyncPrepare(const ChessBoard &board, ReadyHandler handler) ;
    virtual void asyncGetNext(const ChessBoard &board, MoveReadyHandler handler) override;
    virtual void asyncShowMove(const ChessBoard &board, const Move &move, ReadyHandler handler) override;
    virtual void asyncShowResult(const ChessBoard &board, EndStatus status, ReadyHandler handler) override;

    virtual void cancel() override;

    ip::tcp::socket &socket();

  protected:
    std::unique_ptr<ip::tcp::socket> m_sock_ptr;
};

typedef std::shared_ptr<AsyncOnlinePlayer> player_ptr;