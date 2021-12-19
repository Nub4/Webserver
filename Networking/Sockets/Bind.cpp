#include "Bind.hpp"

ft::Bind::Bind(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    // Establish connection
    setConnection(connectToNetwork(getSock(), getAddress()));
    testConnection(getConnection());
}

ft::Bind::~Bind() {}

int     ft::Bind::connectToNetwork(int sock, struct sockaddr_in address)
{
    return bind(sock, (struct sockaddr *)&address, sizeof(address));
}