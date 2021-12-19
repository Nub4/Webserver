#include "Listen.hpp"

ft::Listen::Listen(int domain, int service, int protocol, int port, u_long interface, int backlog)
: Bind(domain, service, protocol, port, interface)
{
    // set backlog variable
    _backlog = backlog;
    // start listening to the network using listen() from sys.socket.h
    startListening();
    // confirm the connection was successful
    testConnection(_listening);
}

ft::Listen::~Listen() {}

// start listening on the network
void    ft::Listen::startListening() { _listening = listen(getSock(), _backlog); }

int     ft::Listen::getListening() { return _listening; }
int     ft::Listen::getBacklog() { return _backlog; }