#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Parsing/Parse.hpp"

class Server : public Parse
{
    public:
        Server(std::vector<serverBlock> servers);
        ~Server();

    private:
        int                             _fdmax;
        std::vector<struct sockaddr_in> _addresses;
        char                            _buffer[BUFF_SIZE];
        std::string                     _content;
        int                             _errorCode;
        std::vector<int>                _serverSockets;
        std::vector<serverBlock> _servers;

        struct sockaddr_in  _getAddress(struct serverBlock server);
        void                _run_server();
        int                 _getSocket(struct sockaddr_in address);
        int                 _find_socket(int i);
        bool                _isNewConnection(int i);
        void                _check(int a, std::string str);
        int                 _accept(int i);
        void                _handler(int clientSocket);
        void                _sendToClient(int clientSocket);
};

#endif