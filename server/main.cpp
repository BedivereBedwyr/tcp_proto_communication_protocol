#include "server.h"

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        server(io_context, std::atoi("31415"));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
