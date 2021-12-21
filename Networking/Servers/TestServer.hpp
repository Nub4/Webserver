#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include "Server.hpp"

class TestServer : public Server
{
    public:
        TestServer();

        void    sendToClient(const char *msg, int len);
        void    launch();

    private:
        char    _buffer[30000];
        int     _newSocket;
        void    _accept();
        void    _handle();
        void    _response();
};

#endif