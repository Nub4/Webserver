#ifndef SERVER_HPP
# define SERVER_HPP

# include "../../includes/libc.hpp"

class Server
{
    public:
        Server();
        ~Server();

        void                run_server();
        void                setup_server();

    private:
        int                 _fdmax;
        struct sockaddr_in  _address;
        int                 _serverSocket;
        void                _check(int a, std::string str);
        char                _buffer[BUFF_SIZE];
        std::string         _content;
        int                 _errorCode;
        int                 _accept();
        void                _handler(int clientSocket);
        void                _sendToClient(int clientSocket);
};

#endif