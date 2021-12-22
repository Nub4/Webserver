#include "Address.hpp"

Address::Address(std::string str)
{
    if (str == "server")
    {
        _address.sin_family = AF_INET;
        _address.sin_port = htons(PORT);
        _address.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
    }
    else if (str == "client")
    {
        memset(&_serv_addr, '0', sizeof(_serv_addr));
        _serv_addr.sin_family = AF_INET;
        _serv_addr.sin_port = htons(PORT);
        // Convert IPv4 from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &_serv_addr.sin_addr) <= 0)
        {
            std::cerr << "\nInvalid address/ Address not supported\n";
            close(getSocket());
            exit(1);
        }
    }
}

struct sockaddr_in  Address::getAddress() { return _address; }
struct sockaddr_in  Address::getServAddr() { return _serv_addr; }