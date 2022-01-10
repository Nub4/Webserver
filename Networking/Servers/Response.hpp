#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Parsing/Parse.hpp"

class Response
{
    public:
        Response();

    protected:
        char            _buffer[BUFF_SIZE];
        int             _errorCode;

        std::string     _root;
        std::string     _index;
        size_t          _max_size;

        std::string     _getStatus(int err_code);
        std::string     _getClientData(std::string type, std::vector<std::string> parsed);
        std::string     _getCacheControl();
        std::string     _getContentType(std::string type);
        std::string     _getContentLength(int size);
        std::string     _getContent(std::vector<std::string> parsed, std::string *type);
        void            _handler(int clientSocket, struct Parse::serverBlock server);
        void            _setBlockData(std::string location, struct Parse::serverBlock server, std::string *type);
};

#endif