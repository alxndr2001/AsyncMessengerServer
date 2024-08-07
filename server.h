#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include "session.h"

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {}
    void start();
private:
    void do_accept(); // Ожидание новых подключений
    tcp::acceptor acceptor_; // Принятие новых соединений
    std::unordered_map<std::string, std::shared_ptr<Session>> sessions_; // Контейнер сессий
};

#endif // SERVER_H
