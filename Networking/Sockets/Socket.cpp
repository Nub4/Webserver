#include "Socket.hpp"

ft::Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
    // Define address structure
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    // Establish socket
    _sock = socket(domain, service, protocol);
    testConnection(_sock);
}

ft::Socket::~Socket() {}

struct sockaddr_in  ft::Socket::getAddress() { return _address; }
int                 ft::Socket::getSock() { return _sock; }

void    ft::Socket::testConnection(int item)
{
    // Confirms that the socker or connection has been properly established
    if (item < 0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}