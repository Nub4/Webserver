#include "Server.hpp"

ft::Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    _socket = new Listen(domain, service, protocol, port, interface, backlog);
}

ft::Server::~Server() { delete _socket; }

ft::Listen  *ft::Server::getSocket() { return _socket; }