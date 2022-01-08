#ifndef SERVER_HPP
# define SERVER_HPP

# include "Response.hpp"

class Server : public Response
{
    public:
        Server(std::vector<Parse::serverBlock> servers);
        ~Server();

    private:
        int                             _fdmax;
        std::vector<struct sockaddr_in> _addresses;
        std::vector<int>                _serverSockets;
        std::vector<Parse::serverBlock> _servers;

        struct sockaddr_in  _getAddress(struct Parse::serverBlock server);
        void                _runServer();
        int                 _getSocket(struct sockaddr_in address);
        int                 _findServer(int i);
        bool                _isNewConnection(int i);
        void                _check(int a, std::string str);
        int                 _accept(int i);
};

#endif