#ifndef SERVER_HPP
# define SERVER_HPP

# include "../ft_libc_network.hpp"

namespace ft
{
    class Server
    {
        public:
            Server(int domain, int service, int protocol, int port, u_long interface, int backlog);
            ~Server();


        private:
            Listen          *_socket;
            virtual void    _accept() = 0;
            virtual void    _handle() = 0;
            virtual void    _response() = 0;
    };
}

#endif