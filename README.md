# tcp_proto_communication_protocol
Basic implementation of a communication protocol using boost.asio sync server/client.

Protocol for messages with a header + body. The header consist of 8 chars, where it will be added the size of the body. This way it is possible to decode many types of messages...

For the server/client engine, it was used the examples of boost.asio(https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/example/cpp11/echo/blocking_tcp_echo_client.cpp and https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/example/cpp11/echo/blocking_tcp_echo_server.cpp).

Please feel free to comment! 
