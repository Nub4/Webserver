#include "Response.hpp"

Response::Response() {}

int     Response::_handler(int clientSocket, struct Parse::serverBlock server)
{
    std::string output;
    int size;
    std::string type;
    char buffer[BUFF_SIZE] = {0};

    if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0)
        return -1;
    std::cout << buffer << std::endl;
    std::istringstream iss(buffer);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    type = parsed[1].substr(parsed[1].rfind(".") + 1, parsed[1].size() - parsed[1].rfind("."));

    _setDefaultData(parsed[1]);
    _setBlockData(parsed, server, &type);

    output = _getClientData(type, parsed);
    size = output.size();
    if (_sendall(clientSocket, output.c_str(), &size) == -1)
    {
        std::cerr << "sendall\n";
        std::cout << "Only " << size << " bytes sended because of the error\n";
    }
    return 1;
}

void    Response::_setDefaultData(std::string location)
{
    _method.clear();
    _error_page.clear();

    _root = "/www/";
    _index = location;
    _max_size = 1048576;
    _errorCode = 200;
    _method.push_back("GET");
    _setErrorPages();
}

void    Response::_setBlockData(std::vector<std::string> parsed, struct Parse::serverBlock server, std::string *type)
{
    int mark = 0;

    if (!server.client_max_body_size.empty())
        _max_size = atoi(server.client_max_body_size.c_str());
    if (!server.error_page.empty())
        for (std::map<int, std::string>::iterator it = server.error_page.begin(); it != server.error_page.end(); it++)
            _error_page[it->first] = it->second;
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
                    size_t i = 0;
                    for (i = 0; i < it->index.size(); i++)
                    {
                        std::ifstream f("." + _root + it->index[i]);
                        if (f.good())
                        {
                            f.close();
                            break;
                        }
                        f.close();
                    }
                    if (i == it->index.size())
                        i--;
                    _index = it->index[i];
                    *type = _index.substr(_index.rfind(".") + 1, _index.size() - _index.rfind("."));
                }
                if (!it->method.empty())
                {
                    _method.pop_back();
                    for (size_t i = 0; i < it->method.size(); i++)
                        _method.push_back(it->method[i]);
                }
            }
        }
    }
    for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); it++)
        if (it->find("Content-Length:") != std::string::npos)
            if (atoi((it + 1)->c_str()) > _max_size)
                _errorCode = 413;
    for (std::vector<std::string>::iterator it = _method.begin(); it != _method.end(); it++)
        if (parsed[0] == *it)
            mark = 1;
    if (mark == 0)
        _errorCode = 405;
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
    oss << content;
    return oss.str();
}

std::string     Response::_getContent(std::vector<std::string> parsed, std::string *type)
{
    std::string content;
    if (_errorCode >= 400 && _errorCode <= 511)
        content = _getErrorPage(type);
    else
    {
        if (parsed[0] == "GET" && parsed[1].size() != 1)
        {
            std::ifstream f("." + _root + _index);
            if (!f.good())
            {
                _errorCode = 404;
                content = _getErrorPage(type); //_get404(type);
            }
            else
                content = _getFile(&f);
            f.close();
        }
        else
            content = _getDefaultFile(type);
    }
    return content;
}