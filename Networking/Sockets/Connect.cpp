#include "Connect.hpp"

ft::Connect::Connect(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    // Establish connection
    setConnection(connectToNetwork(getSock(), getAddress()));
    testConnection(getConnection());
}

ft::Connect::~Connect() {}

int     ft::Connect::connectToNetwork(int sock, struct sockaddr_in address)
{
    return connect(sock, (struct sockaddr *)&address, sizeof(address));
}