#include "Response.hpp"

Response::Response() {}

void    Response::_handler(int clientSocket)
{
    memset(_buffer, 0, sizeof(_buffer));
    recv(clientSocket, _buffer, sizeof(_buffer), 0);
    std::cout << _buffer << std::endl;
    std::istringstream iss(_buffer);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    _errorCode = 200;

    if (parsed.size() >= 3 && parsed[0] == "GET")
    {
        if (parsed[1].size() != 1)
        {
            std::ifstream f("./Pages/" + parsed[1]);
            if (!f.good())
            {
                std::ifstream f2("./Pages/404.html");
                if (f2.good())
                {
                    std::string str((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
                    _content = str;
                    _errorCode = 404;
                }
                f2.close();
            }
            else
            {
                std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                _content = str;
                _errorCode = 200;
            }
            f.close();
        }
        else
        {
            std::ifstream f("./Pages/default.html");
            if (f.good())
            {
                std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                _content = str;
                _errorCode = 200;
            }
            f.close();
        }
    }
    _sendToClient(clientSocket);
}

void    Response::_sendToClient(int clientSocket)
{
    std::string output;
    int size;
    int bytes_sending;
    std::ostringstream oss;
    
    oss << "HTTP/1.1 " << _errorCode << " OK\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-type: text/html\r\n";
    oss << "Content-Length: " << _content.size() << "\r\n";
    oss << "\r\n";
    oss << _content;

    output = oss.str();
    size = output.size();

    bytes_sending = send(clientSocket, output.c_str(), size, 0);
    close(clientSocket);
    if (bytes_sending < 0)
    {
        std::cout << "send\n";
        exit(1);
    }
}

// Response::Response()
// {
//     _contentType = "";
//     _contentLength = "";
//     _cacheControl = "";
// }

// std::string     Response::_get_header(int err_code)
// {
//     std::ostringstream oss;

//     oss << "HTTP/1.1" << err_code << _getStatus(err_code) << "\r\n";
//     oss << _getCacheControl();
//     oss << _getContentType();
//     oss << _getContentLength();
// }

// std::string     Response::_getCacheControl()
// {
    
// }

