#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Parsing/Parse.hpp"

class Server : public Utils
{
    public:
        Server(std::vector<Parse::serverBlock> servers);
        ~Server();

    private:
        int                             _fdmax;
        std::vector<struct sockaddr_in> _addresses;
        char                            _buffer[BUFF_SIZE];
        std::string                     _content;
        int                             _errorCode;
        std::vector<int>                _serverSockets;
        std::vector<Parse::serverBlock> _servers;

        struct sockaddr_in  _getAddress(struct Parse::serverBlock server);
        void                _runServer();
        int                 _getSocket(struct sockaddr_in address);
        int                 _findServer(int i);
        bool                _isNewConnection(int i);
        void                _check(int a, std::string str);
        int                 _accept(int i);
        void                _handler(int clientSocket);
        void                _sendToClient(int clientSocket);
};

#endif