#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

using boost::asio::ip::tcp;

void readFromServer(tcp::socket& socket, std::atomic<bool>& running) {
    try {
        while (running) {
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, '\n');
            std::istream stream(&buffer);
            std::string message;
            std::getline(stream, message);
            std::cout << "Received from server: " << message << std::endl;
        }
    } catch (std::exception& e) {
        if (running) {
            std::cerr << "Exception while reading from server: " << e.what() << "\n";
        }
    }
}

void sendMessage(tcp::socket& socket, const std::string& message) {
    boost::asio::write(socket, boost::asio::buffer(message + "\n"));
}

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::atomic<bool> running(true);
        std::thread readerThread(readFromServer, std::ref(socket), std::ref(running));

        std::string message;
        while (std::getline(std::cin, message)) {
            sendMessage(socket, message);
        }

        running = false;
        readerThread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
