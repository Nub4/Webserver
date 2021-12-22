#include "Listen.hpp"

Listen::Listen(int server_fd)
{
    _listen = listen(server_fd, BACKLOG);
    testConnection(_listen, "listen");
}

int     Listen::getListen() { return _listen; }