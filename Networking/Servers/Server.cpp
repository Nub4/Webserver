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
    fd_set master;
    fd_set writefds;
    fd_set readfds;

    FD_ZERO(&master);
    for (std::vector<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
    {
        FD_SET(*it, &master);
        if (*it > _fdmax)
            _fdmax = *it;
    }
    while (1)
    {
        readfds = writefds = master;
        if (select(_fdmax + 1, &readfds, &writefds, NULL, NULL) < 0)
        {
            std::cerr << RED << "select\n" << RESET;
            continue ;
        }
        for (int i = 0; i <= _fdmax; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (_isNewConnection(i) == true)
                {
                    int clientSocket = _accept(_findServer(i));
                    FD_SET(clientSocket, &master);
                    if (clientSocket > _fdmax)
                        _fdmax = clientSocket;
                }
                else
                {
                    std::map<int, int>::iterator it = _client_server.find(i);
                    _handler(i, _servers[it->second]);
                    close(i);
                    FD_CLR(i, &master);
                    _client_server.erase(i);
                }
            }
        }
    }
}

bool    Server::_isIpAddress(std::string host)
{
    int count = 0;

    for (std::string::iterator it = host.begin(); it != host.end(); it++)
        if (*it == '.')
            count++;
    if (count == 3)
        return true;
    return false;
}

struct sockaddr_in  Server::_getAddress(struct Parse::serverBlock server)
{
    struct sockaddr_in address;
    std::string res;
    int pos = 0;

    if (_isIpAddress(server.listen[1]))
        res = server.listen[1];
    else
    {
        std::ifstream infile("/etc/hosts");
        if (infile.is_open())
        {
            std::string line;
            while (getline(infile, line))
            {
                if (line.find(server.listen[1]) != std::string::npos)
                {
                    pos = line.find(' ');
                    if (line.find("::") == std::string::npos)
                        res = line.substr(0, pos);
                }
            }
        }
        infile.close();
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(server.listen[0].c_str()));
    address.sin_addr.s_addr = inet_addr(res.c_str());
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
    int addrlen = sizeof(_addresses[i]);
    int clientSocket = accept(_serverSockets[i], (struct sockaddr *)&_addresses[i], (socklen_t *)&addrlen);
    if (clientSocket == -1)
        std::cerr << RED << "accept\n" << RESET;
    _client_server[clientSocket] = i;
    return clientSocket;
}

void    Server::_check(int a, std::string str)
{
    if (a < 0)
    {
        std::cerr << RED << str << RESET << std::endl;
        for (std::vector<int>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); it++)
            close(*it);
        exit(1);
    }
}

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
