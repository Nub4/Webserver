#include "RunClient.hpp"

RunClient::RunClient(int server_fd, char *str)
{
    int bytes_sending;
    int valread;

    bytes_sending = send(server_fd, str, strlen(str), 0);
    testConnection(bytes_sending, "send");
    valread = read(server_fd, _buffer, 1024);
    std::cout << _buffer << std::endl;
}