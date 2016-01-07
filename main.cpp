#include "Utils.h"
#include "UDPServer.h"

#include <iostream>

int main(int argc, char* argv[])
{
    int port;
    if(argc != 2)
    {
        return print_usage();
    }

    std::istringstream iss( argv[1] );
    if (!(iss >> port))
    {
        return print_usage();
    }

    try
    {
        boost::asio::io_service io_service;
        UDPServer server(io_service, port);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}