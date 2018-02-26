#include "asynconlineplayer.h"

AsyncOnlinePlayer::AsyncOnlinePlayer(std::unique_ptr<ip::tcp::socket> sock_ptr)
    : m_sock_ptr(std::move(sock_ptr)) {}

void AsyncOnlinePlayer::asyncPrepare(const ChessBoard &board, ReadyHandler handler)
{
    m_strand_ptr->post(handler);
}

ip::tcp::socket &AsyncOnlinePlayer::socket()
{
    return *m_sock_ptr;
}

void AsyncOnlinePlayer::asyncGetNext(const ChessBoard &board, MoveReadyHandler handler)
{
    Move move;
    char raw_data[1024];
    size_t bytes_read = m_sock_ptr->read_some(buffer(raw_data));
    std::string raw_string(raw_data, bytes_read);
    std::stringstream ss;
    ss << raw_string;
    text_iarchive ia{ss};
    ia >> move;
    handler(move);
}
void AsyncOnlinePlayer::asyncShowMove(const ChessBoard &board, const Move &move, ReadyHandler handler)
{
    std::stringstream ss;
    text_oarchive oa{ss};
    oa << move;
    auto raw_data = ss.str();
    m_sock_ptr->write_some(buffer(raw_data));
    handler();
}
void AsyncOnlinePlayer::asyncShowResult(const ChessBoard &board, EndStatus status, ReadyHandler handler)
{
    m_strand_ptr->post(handler);
}

void AsyncOnlinePlayer::cancel()
{
}
