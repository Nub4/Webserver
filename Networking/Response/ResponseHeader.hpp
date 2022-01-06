#ifndef RESPONSEHEADER_HPP
# define RESPONSEHEADER_HPP

# include "../Parsing/Parse.hpp"

class ResponseHeader
{
    public:
        ResponseHeader();

    protected:
        std::string     _contentType;
        std::string     _contentLength;
        std::string     _cacheControl;

        std::string     _get_header();
};

#endif