#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

class ChatClient {
public:
    ChatClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : socket_(io_context) {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket_, resolver.resolve(host, port));
    }

    void write(const std::string& msg) {
        boost::asio::async_write(socket_, boost::asio::buffer(msg + "\n"),
            [](boost::system::error_code ec, std::size_t /*length*/) {
                if (ec) {
                    std::cerr << "Write error: " << ec.message() << std::endl;
                }
            });
    }

    void start_read() {
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(read_msg_), '\n',
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string msg = read_msg_.substr(0, length);
                    read_msg_.erase(0, length);
                    std::cout << "Received: " << msg << std::endl;
                    start_read(); // Continue reading
                } else {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                }
            });
    }

private:
    tcp::socket socket_;
    std::string read_msg_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Usage: chat_client <host> <port> <client_id>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        ChatClient client(io_context, argv[1], argv[2]);

        client.start_read(); // Start reading from server

        std::thread([&io_context]() { io_context.run(); }).detach();

        std::string client_id = argv[3];
        std::string line;
        while (std::getline(std::cin, line)) {
            client.write(client_id + ":" + line); // Send message with client ID
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
