#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Parsing/Parse.hpp"

class Response : public Utils
{
    public:
        Response();

    protected:
        std::string     _getClientData(std::string type, std::vector<std::string> parsed);
        std::string     _getContent(std::vector<std::string> parsed, std::string *type);
        int             _handler(int clientSocket, struct Parse::serverBlock server);
        int             _sendall(int clientSocket, const char *buf, int *size);
        void            _setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type);
        void            _setDefaultData(std::string location);
};

#endif