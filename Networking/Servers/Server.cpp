#include "Server.hpp"

Server::Server(char *conf, std::string path)
{
    Parse parse;

    parse.readConfFile(conf, path);
    parse.getConfigurationData();
    parse.printStructs();
    _server = parse.getServerContent();
}

Server::~Server() { close(_serverSocket); }

void    Server::setup_server()
{
    int yes = 1;
    _fdmax = 0;

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    _check(_serverSocket, "socket");

    _address.sin_family = AF_INET;
    _address.sin_port = htons(atoi(_server[0].listen.c_str()));
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);

    _check(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes), "setsockopt");
    _check(fcntl(_serverSocket, F_SETFL, O_NONBLOCK), "fcntl (F_SETFL)");
    _check(bind(_serverSocket, (struct sockaddr *)&_address, sizeof(_address)), "bind");
    _check(listen(_serverSocket, BACKLOG), "listen");
}

void    Server::run_server()
{
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(_serverSocket, &readfds);
    _fdmax = _serverSocket;

    while (1)
    {
        fd_set copy = readfds;
        _check(select(_fdmax + 1, &copy, NULL, NULL, NULL), "select");

        for (int i = 0; i <= _fdmax; i++)
        {
            if (FD_ISSET(i, &copy))
            {
                if (i == _serverSocket)
                {
                    int clientSocket = _accept();
                    FD_SET(clientSocket, &readfds);
                    if (clientSocket > _fdmax)
                        _fdmax = clientSocket;
                }
                else
                {
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
    memset(_buffer, 0, sizeof(_buffer));
    recv(clientSocket, _buffer, sizeof(_buffer), 0);
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
                std::ifstream f2("./Pages/404.html");
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

void    Server::_sendToClient(int clientSocket)
{
    std::string output;
    int size;
    int bytes_sending;
    std::ostringstream oss;
    
    oss << "HTTP/1.1 " << _errorCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-type: text/html\r\n";
    oss << "Content-Length: " << _content.size() << "\r\n";
    oss << "\r\n";
    oss << _content;

    output = oss.str();
    size = output.size();

    bytes_sending = send(clientSocket, output.c_str(), size, 0);
    _check(bytes_sending, "send");
    close(clientSocket);
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
