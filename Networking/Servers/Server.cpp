#include "Server.hpp"

Server::Server(std::vector<Parse::serverBlock> servers) : _servers(servers)
{
    _fdmax = 0;
    int i = 0;

	for (std::vector<Parse::serverBlock>::iterator it = servers.begin(); it != servers.end(); it++)
	{
        _addresses.push_back(_getAddress(*it));
        _serverSockets.push_back(_getSocket(_addresses[i]));
        if (_serverSockets[i] > _fdmax)
            _fdmax = _serverSockets[i];
        i++;
	}
    _runServer();
}

Server::~Server()
{
    for (std::vector<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
        close(*it);
}

void    Server::_runServer()
{
    fd_set readfds;

    FD_ZERO(&readfds);
    for (std::vector<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
    {
        FD_SET(*it, &readfds);
        if (*it > _fdmax)
            _fdmax = *it;
    }
    while (1)
    {
        fd_set copy = readfds;
        _check(select(_fdmax + 1, &copy, NULL, NULL, NULL), "select");
        for (int i = 0; i <= _fdmax; i++)
        {
            if (FD_ISSET(i, &copy))
            {
                if (_isNewConnection(i) == true)
                {
                    int clientSocket = _accept(_findServer(i));
                    FD_SET(clientSocket, &readfds);
                    if (clientSocket > _fdmax)
                        _fdmax = clientSocket;
                }
                else
                {
                    std::map<int, int>::iterator it = _client_server.find(i);
                    _handler(i, _servers[it->second]);
                    FD_CLR(i, &readfds);
                    _client_server.erase(i);
                }
            }
        }
    }
}

struct sockaddr_in  Server::_getAddress(struct Parse::serverBlock server)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(server.listen[0].c_str()));
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    return address;
}

int     Server::_getSocket(struct sockaddr_in address)
{
    int serverSocket;
    int yes = 1;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    _check(serverSocket, "socket");
    _check(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes), "setsockopt");
    _check(fcntl(serverSocket, F_SETFL, O_NONBLOCK), "fcntl (F_SETFL)");
    _check(bind(serverSocket, (struct sockaddr *)&address, sizeof(address)), "bind");
    _check(listen(serverSocket, BACKLOG), "listen");
    return serverSocket;
}

int     Server::_accept(int i)
{
<<<<<<< HEAD
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
=======
    int addrlen = sizeof(_addresses[i]);
    int clientSocket = accept(_serverSockets[i], (struct sockaddr *)&_addresses[i], (socklen_t *)&addrlen);
    _check(clientSocket, "new socket");
    _client_server[clientSocket] = i;
    return clientSocket;
>>>>>>> main
}

void    Server::_check(int a, std::string str)
{
    if (a < 0)
    {
        std::cerr << str << std::endl;
        for (std::vector<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
            close(*it);
        exit(1);
    }
}

<<<<<<< HEAD
int Server::getServerSocket() { return _serverSocket; }
=======
bool    Server::_isNewConnection(int i)
{
    size_t a = -1;

    while (++a < _serverSockets.size())
        if (i == _serverSockets[a])
            return true;
    return false;
}

int     Server::_findServer(int i)
{
    size_t a = 0;

    while (_serverSockets[a] != i && a < _serverSockets.size())
        a++;
    return a;
}
>>>>>>> main
