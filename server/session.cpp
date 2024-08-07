#include "session.h"
#include "server.h"

void Session::start()
{
    do_read();
}

void Session::deliver(const string &msg)
{
    auto self = shared_from_this();
    boost::asio::post(socket_.get_executor(), [self, msg](){
        self->do_write(msg);
    });
}

tcp::socket &Session::socket() { return socket_; }

void Session::do_read()
{
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(readMessage_), '\n',
    [self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::string msg = self->readMessage_.substr(0, length);
            self->readMessage_.erase(0, length);

            // Extract target client id and message
            auto pos = msg.find(':');
            if (pos != std::string::npos) {
                std::string to_client_id = msg.substr(0, pos);
                std::string message = msg.substr(pos + 1);
                self->server_.deliver("", to_client_id, message); // "" is placeholder for from_client_id
            }

            self->do_read();
        } else {
            // Handle error (e.g., remove client from server)
        }
    });
}

void Session::do_write(const string &msg)
{
    auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (ec) {
                    // Handle error (e.g., remove client from server)
                }
            });
}
