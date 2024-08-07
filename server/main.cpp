#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <memory>

using boost::asio::ip::tcp;
using boost::asio::io_context;
typedef std::vector<unsigned char> buff;

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        async_accept(); // Start accepting coonections
    }

private:
    // Recursive connection accepting
    void async_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    async_accept();
                    handle_client(std::move(socket));
                }
            });
    }
    // Handling client connections
    void handle_client(tcp::socket socket) {
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
    // Sending any data
    void async_send(tcp::socket socket, const std::vector<unsigned char>& data) {
        socket.async_send(boost::asio::buffer(data),
            [socket = std::move(socket)](boost::system::error_code ec, std::size_t /*length*/) {
                if (ec) {
                    std::cerr << "Error on send: " << ec.message() << std::endl;
                }
            });
    }

    void process(std::vector<unsigned char>& data) {
        std::cout << "Received data: " << std::string(data.begin(), data.end()) << std::endl;
    }

    tcp::acceptor acceptor_;
    static uint32_t cliCnt;
};

uint32_t AsyncServer::cliCnt = 0;

int main() {
    try {
        boost::asio::io_context io_context;
        AsyncServer server(io_context, 12345);

        const int threadsNum = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        for(int i = 0; i < threadsNum; ++i)
        {
            threads.emplace_back([&io_context](){
               io_context.run();
            });
        }

        for (auto& thread : threads) {
                    thread.join();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
