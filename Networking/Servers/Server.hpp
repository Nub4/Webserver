#ifndef SERVER_HPP
# define SERVER_HPP

# include "../libc_network.hpp"
# include <unistd.h>

class Server
{
    public:
        Server(int domain, int service, int protocol, int port, u_long interface, int backlog);
        ~Server();

        virtual void    launch() = 0;
        Listen          *getSocket();

    private:
        Listen          *_socket;
        virtual void    _accept() = 0;
        virtual void    _handle() = 0;
        virtual void    _response() = 0;
};

#endif