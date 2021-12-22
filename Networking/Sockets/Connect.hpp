#ifndef CONNECT_HPP
# define CONNECT_HPP

# include "Socket.hpp"

class Connect : public Socket
{
    public:
        Connect(int server_fd, struct sockaddr_in address);

        int     getConnect();

    private:
        int     _connect;
};

#endif