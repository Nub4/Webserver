#include "RunClient.hpp"

RunClient::RunClient(int server_fd)
{
    int bytes_sending;
    int valread;

    _userInput = "Hello from client";

    bytes_sending = send(server_fd, _userInput.c_str(), strlen(_userInput.c_str()), 0);
    testConnection(bytes_sending, "send");
    std::cout << "Hello message sent\n";
    valread = read(server_fd, _buffer, 1024);
    std::cout << _buffer << std::endl;
}