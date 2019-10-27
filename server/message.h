#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <array>
#include <vector>
#include <cstring>
#include <sstream>

struct Message
{
    Message() {}

    Message(const std::string& msg)
        : m_bodySize(msg.length())
    {
        std::cout << "creating message" << std::endl;
        if (m_bodySize > messageSize)
            throw std::runtime_error("can't encode message, too big input");

        // create header
        std::stringstream ss{};
        ss << m_bodySize;
        std::memcpy(&m_header, ss.str().c_str(), ss.str().length());

        // create body
        std::memcpy(&m_body, msg.c_str(), m_bodySize);


        // create message
        std::memcpy(&m_message, &m_header, headerSize);
        std::memcpy(&m_message[headerSize], &m_body, m_bodySize);

        std::cout << "message succesfully created" << std::endl;
    }

    std::size_t size() const
    {
        return headerSize + m_bodySize;
    }

    std::array<char, 1032>* data()
    {
        return &m_message;
    }

    std::string body() const
    {
        std::stringstream ss;
        for (std::size_t i = 0; i < m_bodySize; ++i)
            ss << m_message[headerSize + i];
        return ss.str();
    }

    std::string header() const
    {
        std::stringstream ss;
        for (auto i : m_header)
            ss << i;
        return ss.str();
    }

    static std::size_t maxMessageSize()
    {
        return headerSize + maxBodySize;
    }

    void decodeMessage(char msg[1032])
    {
        std::array<char, 1032> msgArray;
        std::memcpy(msgArray.data(), msg, 1032);
        // create header
        std::memcpy(m_header.data(), &msgArray, headerSize);
        // create body
        std::stringstream ss;
        for (auto i : m_header)
            ss << i;
        ss >> m_bodySize;
        std::memcpy(m_body.data(), &msgArray[headerSize], m_bodySize);
        // create message
        std::memcpy(m_message.data(), m_header.data(), headerSize);
        std::memcpy(&m_message[headerSize], m_body.data(), m_bodySize);
    }

private:
    constexpr static std::size_t headerSize = 8;
    constexpr static std::size_t maxBodySize = 1024;
    constexpr static std::size_t messageSize = headerSize + maxBodySize;

    template<typename T>
    std::string printData(T& data)
    {
        std::cout << "printing data: ";
        std::stringstream ss;
        for (const auto& i : data)
            ss << i;
        return ss.str();
    }

    std::size_t m_bodySize;
    std::array<char, headerSize> m_header;
    std::array<char, maxBodySize> m_body;
    std::array<char, messageSize> m_message;
};

#endif // MESSAGE_H
