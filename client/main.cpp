#include "message.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1032 };

std::string createMessage()
{
    boost::property_tree::ptree pt;
    pt.put("name", "John");
    pt.put("surname", "Doe");
    {
        boost::property_tree::ptree address;
        address.put("street", "John F. Kennedy Street");
        address.put("number", "17");
        address.put("postal_code", "10001");
        pt.add_child("address", address);
    }
    std::stringstream ss{};
    boost::property_tree::write_json(ss, pt, false);
    return ss.str();
}

void sendMessage(Message& msg)
{
    std::cout << "sending message" << std::endl;
    boost::asio::io_context io_context;
    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("127.0.0.1", "31415"));

    // sending to server
    boost::system::error_code ec;
    auto length = s.write_some(boost::asio::buffer(msg.data(), msg.size()), ec);
    if (ec)
        throw std::runtime_error(ec.message());
    if (length != msg.size())
        throw std::runtime_error("something was not sent");

    s.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    s.close();
    std::cout << "message sent" << std::endl;
}

int main()
{
    try
    {
        std::cout << "starting client" << std::endl;
        // send test message
        {
            std::string request = "test message!!";
            Message msg{request};
            sendMessage(msg);
        }

        // send encoded message
        {
            Message msg{createMessage()};
            sendMessage(msg);
        }

        std::cout << "client finished" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
