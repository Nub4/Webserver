#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Parsing/Parse.hpp"

class Response
{
    public:
        Response();

    protected:
        char            _buffer[BUFF_SIZE];
        std::string     _content;
        int             _errorCode;

        // std::string     _contentType;
        // std::string     _contentLength;
        // std::string     _cacheControl;

        // std::string     _get_header();
        void            _handler(int clientSocket);
        void            _sendToClient(int clientSocket);
};

#endif