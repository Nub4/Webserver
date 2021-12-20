#ifndef BIND_HPP
# define BIND_HPP

# include "Socket.hpp"

class Bind : public Socket
{
    public:
        Bind(int domain, int service, int protocol, int port, u_long interface);

        int     getBind();  

    private:
        int     _binding;
        void    connectToNetwork(int sock, struct sockaddr_in address);
};

#endif