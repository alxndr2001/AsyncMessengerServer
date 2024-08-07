#include "server.h"
#include "session.h"
#include <iostream>
#include <map>

AsyncServer::AsyncServer(boost::asio::io_context &io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {}

void AsyncServer::acceptConnection()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket){
        if(!ec) {
            acceptConnection(); // Recursive call to handling next connection

            auto session = std::make_shared<Session>(std::move(socket), *this); // Create new session
            clientId++;
            std::string cliId = std::to_string(clientId);
            std::string message = "Your client id is: " + cliId + "\n";
            boost::asio::async_write(session->socket(), boost::asio::buffer(message),
                [this, session](boost::system::error_code ec, std::size_t /*length*/) {
                    if (ec) {
                        std::cerr << "Cant send clientId to client cause: " << ec.message() << std::endl;
                    }
                });

            addClient(cliId, session);
            session->start();
        } else {
            std::cerr << "Acception error: " << ec.message() << std::endl;
        }
    });
}

void AsyncServer::addClient(const string &id, std::shared_ptr<Session> session)
{
    clients[id] = session; // Add new session
}

void AsyncServer::removeClient(const string &cli_id)
{
    clients.erase(cli_id);
}

void AsyncServer::deliver(const string &from_cli_id, const string &to_cli_id, const string &msg)
{
    auto it = clients.find(to_cli_id);
    if(it != clients.end()) {
        it->second->deliver(from_cli_id + ": " + msg);
    }
}



/*
AsyncServer::AsyncServer(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    async_accept(); // Start accepting coonections
}

void AsyncServer::async_accept() {
    acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            async_accept();
            handle_client(std::move(socket));
        }
    });
}

void AsyncServer::handle_client(tcp::socket socket) {
    tcp::endpoint remote_ep = socket.remote_endpoint();
    std::string client_ip = remote_ep.address().to_string();
    unsigned short client_port = remote_ep.port();

    std::cout << "Client connected! " << std::endl;
    std::cout << "Client info" << std::endl;
    std::cout << "IP: " << client_ip << std::endl;
    std::cout << "Port: " << client_port << std::endl;
    cliCnt++;
    auto buffer = std::make_shared<std::vector<unsigned char>>(1024);
    socket.async_receive(boost::asio::buffer(*buffer),
                         [this, socket = std::move(socket), buffer](boost::system::error_code ec, std::size_t length) mutable {
        if (!ec) {
            buffer->resize(length);
            process(*buffer);
            async_send(std::move(socket), *buffer);
        }
    });
}

void AsyncServer::async_send(tcp::socket socket, const std::vector<unsigned char> &data) {
    socket.async_send(boost::asio::buffer(data),
                      [socket = std::move(socket)](boost::system::error_code ec, std::size_t) {
        if (ec) {
            std::cerr << "Error on send: " << ec.message() << std::endl;
        }
    });
}

void AsyncServer::process(std::vector<unsigned char> &data) {
    std::cout << "Received data: " << std::string(data.begin(), data.end()) << std::endl;
}
*/



