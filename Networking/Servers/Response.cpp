#include "Response.hpp"

Response::Response() {}

void    Response::_handler(int clientSocket, struct Parse::serverBlock server)
{
    std::string output;
    int size;
    int bytes_sending;
    std::string type;

    _errorCode = 200;
    memset(_buffer, 0, sizeof(_buffer));
    recv(clientSocket, _buffer, sizeof(_buffer), 0);
    std::cout << _buffer << std::endl;
    std::istringstream iss(_buffer);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    type = parsed[1].substr(parsed[1].rfind(".") + 1, parsed[1].size() - parsed[1].rfind("."));

    _setBlockData(parsed, server, &type);

    output = _getClientData(type, parsed);
    size = output.size();
    bytes_sending = send(clientSocket, output.c_str(), size, 0);
    close(clientSocket);
}



void    Response::_setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type)
{
    _root = "/www/";
    _index = parsed[1];
    if (!server.client_max_body_size.empty())
        _max_size = atoi(server.client_max_body_size.c_str());
    else
        _max_size = 1048576;
    if (!server.location.empty())
    {
        for (std::vector<Parse::locationBlock>::iterator it = server.location.begin(); it != server.location.end(); it++)
        {
            if (it->name == parsed[1])
            {
                if (!it->root.empty())
                    _root = it->root;
                if (!it->index.empty())
                {
                    _index = it->index[0];
                    *type = _index.substr(_index.rfind(".") + 1, _index.size() - _index.rfind("."));
                }
            }
        }
    }
    for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); it++)
        if (it->find("Content-Length:") != std::string::npos)
            if (atoi((it + 1)->c_str()) > _max_size)
                _errorCode = 413;
}

std::string     Response::_getClientData(std::string type, std::vector<std::string> parsed)
{
    std::ostringstream oss;
    std::string content;

    content = _getContent(parsed, &type);    
    oss << "HTTP/1.1 " << _errorCode << _getStatus(_errorCode);
    oss << _getCacheControl();
    oss << _getContentType(type);
    oss << _getContentLength(content.size());
    oss << "\r\n";
//   std::cout << oss.str() << std::endl;
    oss << content;
    return oss.str();
}

std::string     Response::_getContent(std::vector<std::string> parsed, std::string *type)
{
    std::string content;
    if (_errorCode == 413)
        content = _get413(type);
    else
    {
        if (parsed[0] == "GET" && parsed[1].size() != 1)
        {
            std::ifstream f("." + _root + _index);
            if (!f.good())
                content = _get404(type);
            else
                content = _getFile(&f);
            f.close();
        }
        else
            content = _getDefaultFile(type);
    }
    return content;
}
std::string Response::_get413(std::string *type)
{
    std::string content;
    std::ifstream f("./www/413.html");
    if (f.good())
    {
        std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        content = str;
        _errorCode = 413;
        *type = "html";
    }
    f.close();
    return content;
}

std::string Response::_getFile(std::ifstream *f)
{
    std::string str((std::istreambuf_iterator<char>(*f)), std::istreambuf_iterator<char>());
    std::string content = str;
    _errorCode = 200;
    return content;
}

std::string Response::_get404(std::string *type)
{
    std::string content;
    std::ifstream f2("./www/404.html");
    if (f2.good())
    {
        std::string str((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
        content = str;
        _errorCode = 404;
        *type = "html";
    }
    f2.close();
    return content;
}

std::string Response::_getDefaultFile(std::string *type)
{
    std::string content;
    std::ifstream f("./www/default.html");
    if (f.good())
    {
        std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        content = str;
        _errorCode = 200;
        *type = "html";
    }
    f.close();
    return content;
}


std::string     Response::_getContentLength(int size)
{
    std::ostringstream oss;

    oss << "Content-Length: ";
    oss << size << "\r\n";
    return oss.str();
}

std::string     Response::_getCacheControl()
{
    std::string str;

    str = "Cache-Control: no-cache, private\r\n";
    return str;
}

std::string     Response::_getStatus(int err_code)
{
    if (err_code == 200)
        return " OK\r\n";
    else if (err_code == 413)
        return "Payload Too Large\r\n";
    else
        return " Not Found\r\n";
}

std::string     Response::_getContentType(std::string type)
{
    std::string str;

    str = "Content-Type: ";
    if (type == "jpeg" || type == "jpg")
        str += "image/jpeg";
    else if (type == "html" || type == "/")
        str += "text/html";
    else if (type == "png")
        str += "image/png";
    else if (type == "bmp")
        str += "image/bmp";
    else
        str += "text/plain";
    str += "\r\n";
    return str;
}