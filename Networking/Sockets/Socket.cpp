#include "Socket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
    _sock = socket(domain, service, protocol);
    testConnection(_sock, "sock");
}

struct sockaddr_in  Socket::getAddress() { return _address; }
int                 Socket::getSock() { return _sock; }

void    Socket::testConnection(int item, std::string s)
{
    if (item < 0)
    {
        std::cerr << s << std::endl;
        exit(EXIT_FAILURE);
    }
}