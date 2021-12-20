#include "TestServer.hpp"

ft::TestServer::TestServer()
: Server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10) { launch(); }

ft::TestServer::~TestServer() {}

void    ft::TestServer::_accept()
{
    struct sockaddr_in address = getSocket()->getAddress();
    int addrlen = sizeof(address);
    _newSocket = accept(getSocket()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    read(_newSocket, _buffer, 30000);
}

void    ft::TestServer::_handle()
{
    std::cout << _buffer << std::endl;
}

void    ft::TestServer::_response()
{
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nHello world! I'm here!";
   // char *hello = "Hello from Server";
    write(_newSocket, hello, strlen(hello));
    close(_newSocket);
}

void    ft::TestServer::launch()
{
    while (1)
    {
        std::cout << "======= WAITING ======\n";
        _accept();
        _handle();
        _response();
        std::cout << "======= DONE ======\n";
    }
}