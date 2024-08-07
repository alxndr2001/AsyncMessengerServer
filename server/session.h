#ifndef SESSION_H
#define SESSION_H
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>
#include <boost/asio.hpp>
#include "server.h"

using namespace std;
using boost::asio::ip::tcp;

class AsyncServer;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, AsyncServer& server)
            : socket_(std::move(socket)), server_(server) {}
    void start(); // Start session
    void deliver(const string &msg); //Sending to another client
    tcp::socket& socket(); // Метод для доступа к сокету

private:
    void do_read();
    void do_write(const string &msg);

    tcp::socket socket_; // Client sock connection
    std::string readMessage_;
    AsyncServer& server_;
};

#endif // SESSION_H
