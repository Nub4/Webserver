#include "Bind.hpp"

Bind::Bind(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    connectToNetwork(getSock(), getAddress());
    testConnection(_binding);
}

void     Bind::connectToNetwork(int sock, struct sockaddr_in address){
    int yes = 1;
    
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    _binding = bind(sock, (struct sockaddr *)&address, sizeof(address));
}

int     Bind::getBind() { return _binding; }