#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../Parsing/Parse.hpp"

class Response
{
    public:
        Response();

    protected:
        std::string     _contentType;
        std::string     _contentLength;
        std::string     _cacheControl;

        std::string     _get_header();
};

#endif