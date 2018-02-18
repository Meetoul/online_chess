#include "asyncclientplayer.hpp"

AsyncClientPlayer::AsyncClientPlayer(std::shared_ptr<io_service> io_ptr)
    : m_socket(*io_ptr) {}

ip::tcp::socket &AsyncClientPlayer::socket()
{
    return m_socket;
}

void AsyncClientPlayer::asyncPrepare(const ChessBoard &board, ReadyHandler handler)
{
    handler();
}

void AsyncClientPlayer::asyncGetNext(const ChessBoard &board, MoveReadyHandler handler)
{
    Move move;
    char raw_data[1024];
    size_t bytes_read = m_socket.read_some(buffer(raw_data));
    std::string raw_string(raw_data, bytes_read);
    std::stringstream ss;
    ss << raw_string;
    text_iarchive ia{ss};
    ia >> move;
    handler(move);
}
void AsyncClientPlayer::asyncShowMove(const ChessBoard &board, const Move &move, ReadyHandler handler)
{
    std::stringstream ss;
    text_oarchive oa{ss};
    oa << move;
    auto raw_data = ss.str();
    m_socket.write_some(buffer(raw_data));
    handler();
}
void AsyncClientPlayer::asyncShowResult(const ChessBoard &board, EndStatus status, ReadyHandler handler)
{
    handler();
}

void AsyncClientPlayer::cancel()
{
}
