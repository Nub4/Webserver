#ifndef LISTEN_HPP
# define LISTEN_HPP

# include "../../includes/libc.hpp"
# include "Socket.hpp"

class Listen : public Socket
{
    public:
        Listen(int server_fd);

        int     getListen();

    private:
        int _listen;
};

#endif