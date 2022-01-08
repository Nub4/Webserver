#include "Response.hpp"

Response::Response()
{
    _contentType = "";
    _contentLength = "";
    _cacheControl = "";
}

std::string     Response::_get_header(int err_code)
{
    std::ostringstream oss;

    oss << "HTTP/1.1" << err_code << _getStatus(err_code) << "\r\n";
    oss << _getCacheControl();
    oss << _getContentType();
    oss << _getContentLength();
}

std::string     Response::_getCacheControl()
{
    
}

