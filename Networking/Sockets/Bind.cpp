#include "Bind.hpp"

Bind::Bind(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    connectToNetwork(getSock(), getAddress());
    testConnection(_binding);
}

void     Bind::connectToNetwork(int sock, struct sockaddr_in address){
    _binding = bind(sock, (struct sockaddr *)&address, sizeof(address));
}

int     Bind::getBind() { return _binding; }