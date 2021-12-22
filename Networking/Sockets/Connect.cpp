#include "Connect.hpp"

Connect::Connect(int server_fd, struct sockaddr_in serv_addr)
{
    _connect = connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    testConnection(_connect, "connect");
}

int     Connect::getConnect() { return _connect; }