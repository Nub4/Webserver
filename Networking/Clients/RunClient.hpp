#ifndef RUNCLIENT_HPP
# define RUNCLIENT_HPP

# include "../../includes/libc.hpp"

class RunClient : public Socket
{
    public:
        RunClient(int server_fd);

    private:
        char    _buffer[1024];
};

#endif