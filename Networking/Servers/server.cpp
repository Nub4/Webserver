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
    while (1)
    {
        _accept(_socket, _address);
        _handler(_socket);
    }
}

void    Server::_accept(int server_fd, struct sockaddr_in address)
{
    int addrlen = sizeof(address);
    _newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    _check(_newSocket, "new socket");
}

void    Server::_handler(int server_fd)
{
    if (!fork())
    {
        close(server_fd);
        //recv(_newSocket, _buffer, sizeof(_buffer), 0);
        read(_newSocket, _buffer, sizeof(_buffer));//30000);
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
        _sendToClient();
    }
    close(_newSocket);
    
}

void    Server::_sendToClient()
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

    int bytes_sending = send(_newSocket, output.c_str(), size, 0);
    _check(bytes_sending, "send");
    close(_newSocket);
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