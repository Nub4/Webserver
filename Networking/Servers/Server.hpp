#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/libc.hpp"

class Server
{
    public:
        Server();

        void                run_server();
        void                setup_server();

    private:
        struct sockaddr_in  _address;
        int                 _socket;
        int                 _bind;
        int                 _listen;
        void                _check(int a, std::string str);
        char                _buffer[30000];
        int                 _newSocket;
        std::string         _content;
        int                 _errorCode;
        void                _accept(int server_fd, struct sockaddr_in address);
        void                _handler(int server_fd);
        void                _sendToClient();
};

#endif