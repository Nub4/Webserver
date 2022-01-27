#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Parsing/Parse.hpp"
# include "CGI.hpp"
# include <cstdio>

class Response : public Utils
{
    public:
        Response();

    protected:
        std::string     _getClientData(std::string type, std::vector<std::string> parsed, struct Parse::serverBlock server);
        std::string     _getContent(std::vector<std::string> parsed, std::string *type);
        void            _handler(int clientSocket, struct Parse::serverBlock server);
        void            _setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type);
        void            _setDefaultData(std::string location);
		void			_createHeader(std::ostringstream &oss, int _errorCode, std::string type, size_t content_length);
};

#endif