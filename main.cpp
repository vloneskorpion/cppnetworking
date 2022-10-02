#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(1024);

void GrabData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if(!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for(int i = 0; i < length; i++)
                    std::cout << vBuffer[i];

                GrabData(socket);
            }
        });
}

int main()
{
    // Create error handler
    asio::error_code ec;

    // Create a asio contex
    asio::io_context context;

    //Give some fake tasks to asio
    asio::io_context::work idleWork(context);

    //Start the context
    std::thread thrContext = std::thread([&]() {context.run(); });

    //Get the adress of something we want to connect
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

    // Create a socket
    asio::ip::tcp::socket socket(context);

    //Tell socket to try to connect
    socket.connect(endpoint, ec);

    !ec ? std::cout << "Connected!\n" : std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;

    if(socket.is_open())
    {
        GrabData(socket);

        std::string sRequest =
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        //Program does somethinf else, while asio handles data transfer in background
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

        context.stop();
        if(thrContext.joinable()) thrContext.join();


    } else { std::cout << "Socket is not open!\n"; }

    return 0;
}