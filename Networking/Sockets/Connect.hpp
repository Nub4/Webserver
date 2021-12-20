#ifndef CONNECT_HPP
# define CONNECT_HPP

#include "Socket.hpp"

class Connect : public Socket
{
    public:
        Connect(int domain, int service, int protocol, int port, u_long interface);

        void connectToNetwork(int sock, struct sockaddr_in address);
};

#endif