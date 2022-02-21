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
        std::string     _checkCorrectErrorPage(std::string original, std::string *type);
        std::string     _getInsertedErrorPage(std::ifstream *f, int errCode);
        int             _fileUpload(char *s, int clientSocket);
        std::string     _getClientData(std::string type, std::vector<std::string> parsed, struct Parse::serverBlock server);
        std::string     _getContent(std::vector<std::string> parsed, std::string *type);
        void            _handler(int clientSocket, struct Parse::serverBlock server);
        void            _setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type);
        void            _setDefaultData(std::string location);
		void			_createHeader(std::ostringstream &oss, int _errorCode, std::string type, size_t content_length);
		bool			_typeIsPy(std::string type);
        bool            _is404(struct Parse::serverBlock server, std::string location);
};

#endif