#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include "Server.hpp"

namespace ft
{
    class TestServer : public Server
    {
        public:
            TestServer();
            ~TestServer();

            void    launch();

        private:
            char    _buffer[3000] = {0};
            int     _newSocket;
            void    _accept();
            void    _handle();
            void    _response();
    };
}

#endif