#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <iostream>

class Socket
{
    public:
        Socket();
        Socket(int domain, int type, int protocol);

        int     getSocket();
        void    testConnection(int item, std::string str);

    private:
        int                 _socket;
};

#endif