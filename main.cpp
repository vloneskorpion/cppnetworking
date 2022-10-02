#include <iostream>
#include <string>
#include <chrono>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
    // Create error handler
    asio::error_code ec;

    // Create a asio contex
    asio::io_context context;

    //Get the adress of something we want to connect
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

    // Create a socket
    asio::ip::tcp::socket socket(context);

    //Tell socket to try to connect
    socket.connect(endpoint, ec);

    !ec ? std::cout << "Connected!\n" : std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;

    if(socket.is_open())
    {
        std::string sRequest =
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(200ms);

        size_t bytes = socket.available();
        std::cout << "Bytes Available: " << bytes << std::endl;

        if(bytes > 0)
        {
            std::vector<char> vBuffer(bytes);
            socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

            for (const auto& c : vBuffer) std::cout << c;
        }
    }

    return 0;
}