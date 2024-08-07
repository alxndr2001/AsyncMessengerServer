#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "server.h"
#include "session.h"

int main() {
    try {
        boost::asio::io_context io_context;
        AsyncServer server(io_context, 12345);
        std::cout << "Server started";
        server.acceptConnection();

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
