#include "Server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog){
    _socket = new Listen(domain, service, protocol, port, interface, backlog);
}

Server::~Server() { delete _socket; }

Listen  *Server::getSocket() { return _socket; }