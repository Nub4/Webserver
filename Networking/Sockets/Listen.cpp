#include "Listen.hpp"

Listen::Listen(int domain, int service, int protocol, int port, u_long interface, int backlog)
: Bind(domain, service, protocol, port, interface)
{
    _backlog = backlog;
    startListening();
    testConnection(_listening, "listen");
}

Listen::~Listen() {}

void    Listen::startListening() { _listening = listen(getSock(), _backlog); }

int     Listen::getListening() { return _listening; }
int     Listen::getBacklog() { return _backlog; }