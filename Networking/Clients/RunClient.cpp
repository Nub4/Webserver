#include "RunClient.hpp"

RunClient::RunClient(int server_fd)
{
    int bytes_sending;
    int valread;

    bytes_sending = send(server_fd, "Hello from client", strlen("Hello from client"), 0);
    testConnection(bytes_sending, "send");
    std::cout << "Hello message sent\n";
    valread = read(server_fd, _buffer, 1024);
    std::cout << _buffer << std::endl;
}