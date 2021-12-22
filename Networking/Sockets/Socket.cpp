#include "Socket.hpp"

Socket::Socket() {}

Socket::Socket(int domain, int type, int protocol)
{
    _socket = socket(domain, type, protocol);
    testConnection(_socket, "socket");
}

void    Socket::testConnection(int item, std::string str)
{
    if (item < 0)
    {
        std::cerr << str << std::endl;
        close(getSocket());
        exit(1);
    }
}

int     Socket::getSocket() { return _socket; }