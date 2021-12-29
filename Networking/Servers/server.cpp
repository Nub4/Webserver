#include "Server.hpp"

Server::Server() {}

void    Server::setup_server()
{
    int yes = 1;
    int ss;

    // Socket file descriptor
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    _check(_socket, "socket");

    // Initialize the address struct
    _address.sin_family = AF_INET;
    _address.sin_port = htons(PORT);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

    // Binding
    ss = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    _check(ss, "setsockopt");
    _bind = bind(_socket, (struct sockaddr *)&_address, sizeof(_address));
    _check(_bind, "bind");

    // Listen
    _listen = listen(_socket, BACKLOG);
    _check(_listen, "listen");
}

void    Server::run_server()
{
    fd_set current_sockets;
    fd_set ready_sockets; // copy of current sockets

    // Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(_socket, &current_sockets);
 
    while (1)
    {
        ready_sockets = current_sockets;
        _check(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "select");

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == _socket)
                {
                    // this is a new connection
                    int newSocket = _accept();
                    FD_SET(newSocket, &current_sockets);
                }
                else
                {
                    // do whatever do with connections.
                    _handler(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }
}

int     Server::_accept()
{
    int addrlen = sizeof(_address);
    int newSocket = accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
    _check(newSocket, "new socket");
    return newSocket;
}

void    Server::_handler(int newSocket)
{
    if (!fork())
    {
        close(_socket);
        //recv(_newSocket, _buffer, sizeof(_buffer), 0);
        read(newSocket, _buffer, sizeof(_buffer));//30000);
        std::cout << _buffer << std::endl;
        std::istringstream iss(_buffer);
        std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
        _errorCode = 200;

        if (parsed.size() >= 3 && parsed[0] == "GET")
        {
            if (parsed[1].size() != 1)
            {
                std::ifstream f("./Pages/" + parsed[1]);
                if (!f.good())
                {
                    std::ifstream f2("./Pages/error.html");
                    if (f2.good())
                    {
                        std::string str((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
                        _content = str;
                        _errorCode = 404;
                    }
                    f2.close();
                }
                else
                {
                    std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                    _content = str;
                    _errorCode = 200;
                }
                f.close();
            }
            else
            {
                std::ifstream f("./Pages/default.html");
                if (f.good())
                {
                    std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                    _content = str;
                    _errorCode = 200;
                }
                f.close();
            }
        }
        _sendToClient(newSocket);
    }
    close(newSocket);
    
}

void    Server::_sendToClient(int newSocket)
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << _errorCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-type: text/html\r\n";
    oss << "Content-Length: " << _content.size() << "\r\n";
    oss << "\r\n";
    oss << _content;

    std::string output = oss.str();
    int size = output.size();

    int bytes_sending = send(newSocket, output.c_str(), size, 0);
    _check(bytes_sending, "send");
    close(newSocket);
    exit(0);
}

void    Server::_check(int a, std::string str)
{
    if (a < 0)
    {
        std::cerr << str << std::endl;
        if (_socket)
            close(_socket);
        exit(1);
    }
}