#ifndef BIND_HPP
# define BIND_HPP

# include "Socket.hpp"

namespace ft
{
    class Bind : public Socket
    {
        public:
            Bind(int domain, int service, int protocol, int port, u_long interface);
            ~Bind();

            int connectToNetwork(int sock, struct sockaddr_in address);  
    };
}

#endif