#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Parsing/Parse.hpp"

class Server : public Parse
{
    public:
        Server(serverBlock server);
        ~Server();

        void                run_server();
        void                setup_server();
		int		getServerSocket();

    private:
        int                         _fdmax;
        struct sockaddr_in          _address;
        int                         _serverSocket;
        char                        _buffer[BUFF_SIZE];
        std::string                 _content;
        int                         _errorCode;
        serverBlock				    _server;

        void    _check(int a, std::string str);
        int     _accept();
        void    _handler(int clientSocket);
        void    _sendToClient(int clientSocket);
};

#endif