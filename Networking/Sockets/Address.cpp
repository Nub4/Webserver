#include "Address.hpp"

Address::Address()
{
    _address.sin_family = AF_INET;
    _address.sin_port = htons(PORT);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

struct sockaddr_in  Address::getAddress() { return _address; }