#include "Listen.hpp"

ft::Listen::Listen(int domain, int service, int protocol, int port, u_long interface, int backlog)
: Bind(domain, service, protocol, port, interface)
{
    _backlog = backlog;
    startListening();
    testConnection(_listening);
}

void    ft::Listen::startListening() { _listening = listen(getConnection(), _backlog); }