#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include "session.h"

class Session;

using boost::asio::ip::tcp;
//using boost::asio::io_context;
//typedef std::vector<unsigned char> buff;

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io_context, short port);

    void addClient(const std::string& id, std::shared_ptr<Session> session);
    void removeClient(const std::string& cli_id);
    void deliver(const std::string& from_cli_id, const std::string& to_cli_id, const std::string& msg);
    void acceptConnection();

private:

    tcp::acceptor acceptor_;
    uint32_t clientId;
    std::unordered_map<std::string, std::shared_ptr<Session>> clients;
    /*
    // Recursive connection accepting
    void async_accept();
    // Handling client connections
    void handle_client(tcp::socket socket);
    // Sending any data
    void async_send(tcp::socket socket, const std::vector<unsigned char>& data);

    void process(std::vector<unsigned char>& data);

    tcp::acceptor acceptor_;
    uint32_t cliCnt;
    */
};


#endif // SERVER_H
