#ifndef SERVER_H
#define SERVER_H

#include "message.h"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int max_length = 1032;

void session(tcp::socket sock)
{
    try
    {
        std::cout << "incoming message" << std::endl;
        while (true)
        {
            char data[max_length];
            boost::system::error_code error;
            auto length = sock.read_some(boost::asio::buffer(data), error);
            if (length == max_length || error == boost::asio::error::eof) {
                Message msg;
                msg.decodeMessage(data);
                std::cout << "message received: " << msg.body() << std::endl;
                break;
            }
            else if (error)
                throw boost::system::system_error(error); // Some other error.
        }
        std::cout << "message received" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    for (;;)
        {
            std::cout << "starting server! waiting for messages..." << std::endl;
            std::thread(session, a.accept()).detach();
        }
}

#endif // SERVER_H
