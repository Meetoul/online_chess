#include "asyncserverplayer.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace boost::archive;

AsyncServerPlayer::AsyncServerPlayer(std::shared_ptr<io_service> io_ptr)
    : m_socket(*io_ptr) {}

ip::tcp::socket &AsyncServerPlayer::socket()
{
    return m_socket;
}

void AsyncServerPlayer::asyncPrepare(const ChessBoard &board, ReadyHandler handler)
{
    char color_buffer[1];
    m_socket.read_some(buffer(color_buffer));
    handler();
}

void AsyncServerPlayer::asyncGetNext(const ChessBoard &board, MoveReadyHandler handler)
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
void AsyncServerPlayer::asyncShowMove(const ChessBoard &board, const Move &move, ReadyHandler handler)
{
    std::stringstream ss;
    text_oarchive oa{ss};
    oa << move;
    auto raw_data = ss.str();
    m_socket.write_some(buffer(raw_data));
    handler();
}
void AsyncServerPlayer::asyncShowResult(const ChessBoard &board, EndStatus status, ReadyHandler handler)
{
    handler();
}

void AsyncServerPlayer::cancel()
{
}
