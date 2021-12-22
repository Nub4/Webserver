#ifndef BIND_HPP
# define BIND_HPP

# include "Socket.hpp"

class Bind : public Socket
{
    public:
        Bind(int server_fd, struct sockaddr_in address);

        int     getBind();

    private:
        int     _bind;
};

#endif