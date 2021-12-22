#include "Bind.hpp"

Bind::Bind(int server_fd, struct sockaddr_in address)
{
    int yes = 1;
    int n;

    n = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    testConnection(n, "setsockopt");
    _bind = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    testConnection(_bind, "bind");
}

int     Bind::getBind() { return _bind; }