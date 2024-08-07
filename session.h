#ifndef SESSION_H
#define SESSION_H
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

class Session;
typedef std::unordered_map<std::string, std::shared_ptr<Session>> sessionsMap;

class Session : public std::enable_shared_from_this<Session> {
    Session(tcp::socket socket, sessionsMap& sessions)
            : socket_(std::move(socket)), sessions_(sessions) {}
    void start(); // Запуск сессии

private:
    void do_read(); // Чтение данных с клиента

    tcp::socket socket_; // Сокет соединения с клиентом
    std::string id; // Идентификатор сессии
    sessionsMap sessions_;
};

#endif // SESSION_H
