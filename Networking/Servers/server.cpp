#include "Server.hpp"

Server::Server() {}

void    Server::setup_server()
{
    int yes = 1;

    // Socket file descriptor
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    _check(_serverSocket, "socket");

    // Initialize the address struct
    _address.sin_family = AF_INET;
    _address.sin_port = htons(PORT);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

    // Binding
    _check(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes), "setsockopt");
    _check(bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address)), "bind");

    // Listen
    _check(listen(_serverSocket, BACKLOG), "listen");
}

void    Server::run_server()
{
    fd_set readfds;
    fd_set tmp_readfds;

    // Initialize my current set
    FD_ZERO(&readfds);
    FD_SET(_serverSocket, &readfds);
 
    while (1)
    {
        tmp_readfds = readfds;
        _check(select(FD_SETSIZE, &tmp_readfds, NULL, NULL, NULL), "select");

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &tmp_readfds))
            {
                if (i == _serverSocket)
                {
                    // this is a new connection
                    int clientSocket = _accept();
                    FD_SET(clientSocket, &readfds);
                }
                else
                {
                    // do whatever do with connections.
                    _handler(i);
                    FD_CLR(i, &readfds);
                }
            }
        }
    }
}

int     Server::_accept()
{
    int addrlen = sizeof(_address);
    int clientSocket = accept(_serverSocket, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
    _check(clientSocket, "new socket");
    return clientSocket;
}

void    Server::_handler(int clientSocket)
{
    if (!fork())
    {
        close(_serverSocket);
        //recv(_newSocket, _buffer, sizeof(_buffer), 0);
        read(clientSocket, _buffer, sizeof(_buffer));//30000);
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
        _sendToClient(clientSocket);
    }
    close(clientSocket);
    
}

void    Server::_sendToClient(int clientSocket)
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

    int bytes_sending = send(clientSocket, output.c_str(), size, 0);
    _check(bytes_sending, "send");
    close(clientSocket);
    exit(0);
}

void    Server::_check(int a, std::string str)
{
    if (a < 0)
    {
        std::cerr << str << std::endl;
        if (_serverSocket)
            close(_serverSocket);
        exit(1);
    }
}