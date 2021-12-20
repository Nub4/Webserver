#include "Connect.hpp"

Connect::Connect(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    setConnection(connectToNetwork(getSock(), getAddress()));
    testConnection(getConnection());
}

int     Connect::connectToNetwork(int sock, struct sockaddr_in address){
    return connect(sock, (struct sockaddr *)&address, sizeof(address));
}