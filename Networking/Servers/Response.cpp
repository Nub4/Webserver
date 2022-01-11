#include "Response.hpp"

Response::Response() {}

void    Response::_handler(int clientSocket, struct Parse::serverBlock server)
{
    std::string output;
    int size;
    int bytes_sending;
    std::string type;
    char buffer[BUFF_SIZE] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << buffer << std::endl;
    std::istringstream iss(buffer);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    type = parsed[1].substr(parsed[1].rfind(".") + 1, parsed[1].size() - parsed[1].rfind("."));

    _setDefaultData(parsed[1]);
    _setBlockData(parsed, server, &type);

    output = _getClientData(type, parsed);
    size = output.size();
    bytes_sending = send(clientSocket, output.c_str(), size, 0);
    close(clientSocket);
}

void    Response::_setDefaultData(std::string location)
{
    _root = "/www/";
    _index = location;
    _max_size = 1048576;
    _errorCode = 200;
}

void    Response::_setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type)
{
    if (!server.client_max_body_size.empty())
        _max_size = atoi(server.client_max_body_size.c_str());
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