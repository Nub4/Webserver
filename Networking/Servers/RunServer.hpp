#ifndef RUNSERVER_HPP
# define RUNSERVER_HPP

# include "../../includes/libc.hpp"

class RunServer : public Socket
{
    public:
        RunServer(int server_fd, struct sockaddr_in address);

    private:
        char        _buffer[30000];
        int         _newSocket;
        std::string _content;
        int         _errorCode;
        void        _accept(int server_fd, struct sockaddr_in address);
        void        _handler(int server_fd);
        void        _sendToClient();
};

#endif