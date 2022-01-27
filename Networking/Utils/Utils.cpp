#include "Utils.hpp"

Utils::Utils() {}

std::vector<unsigned char>  Utils::readBinaryFile(char *conf, std::string path)
{
    std::string filename = conf;
    std::string str = path + "/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    return binary_file;
}

/* 
* These ones
* are for
* Parse class:
*/

int     Utils::_checkClosingBracket(int pos, std::string s)
{
    int mark = 1;
    size_t i;

    for (i = pos; i < s.size() && mark != 0; i++)
    {
        if (s[i] == '{')
            mark++;
        else if (s[i] == '}')
            mark--;
    }
    if (i == s.size())
        return 0;
    return i;
}

bool    Utils::_isCorrectHost(std::string host)
{
    int count = 0;

    for (std::string::iterator it = host.begin(); it != host.end(); it++)
        if (*it == '.')
            count++;
    if (count == 3)
        return true;
    std::ifstream infile("/etc/hosts");
    if (infile.is_open())
    {
        std::string line;
        while (getline(infile, line))
        {
            if (line.find(host) != std::string::npos)
            {
                infile.close();
                return true;
            }
        }
    }
    infile.close();
    return false;
}

void    Utils::_checkBackChar(std::string *x, std::string name)
{
    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, " + name);
    x->pop_back();
}

void    Utils::_check_listen(std::vector<std::string> *v)
{
    _checkBackChar(&v->back(), "listen");
    if ((*v).size() == 1)
    {
        if (_isCorrectHost((*v)[0]))
            v->insert(v->begin(), "8080");
        else
            v->push_back("127.0.0.1");
    }
    if ((*v).size() > 2 || !_isNumber((*v)[0]) || atoi((*v)[0].c_str()) < 0 || atoi((*v)[0].c_str()) > 65535)
        _msg_exit("configuration file error, listen");
}

void    Utils::_check_error_page(std::map<int, std::string> *m)
{
    std::map<int, std::string>::iterator it = m->begin();
    if ((it->first != 404 && it->second.back() != ';') || std::count(it->second.begin(), it->second.end(), ';') != 1)
        _msg_exit("configuration file error, error_page");
    it->second.pop_back();
}

void    Utils::_check_redirect(std::vector<std::string> *v)
{
    _checkBackChar(&v->back(), "return");
    if ((*v)[0] != "301" && v->size() != 2)
        _msg_exit("configuration file error, return");
}

void    Utils::_check_autoindex(std::string *x)
{
    if (*x != "on;" && *x != "off;")
        _msg_exit("configuration file error, autoindex");
    x->pop_back();
}

void    Utils::_check_method(std::vector<std::string> *v)
{
    size_t size = v->size();
    _checkBackChar(&v->back(), "method");
    for (size_t i = 0; i < v->size(); i++)
    {
        if ((*v)[i] != "GET" && (*v)[i] != "POST" && (*v)[i] != "DELETE")
            _msg_exit("configuration file error, methods");
        for (size_t j = i + 1; j < size; j++)
            if ((*v)[i] == (*v)[j])
                _msg_exit("configuration file error, methods");
    }
}

void    Utils::_check_body_size(std::string *x)
{
    int tmp = 0;
    std::stringstream ss;

    _checkBackChar(x, "client_max_body_size");
    if (x->back() == 'M')
    {
        x->pop_back();
        if (!_isNumber(*x))
            _msg_exit("configuration file error, client_max_body_size");
        tmp = atoi(x->c_str()) * 1048576;
        ss << tmp;
        ss >> *x;
    }
    else if (x->back() == 'K')
    {
        x->pop_back();
        if (!_isNumber(*x))
            _msg_exit("configuration file error, client_max_body_size");
        tmp = atoi(x->c_str()) * 1024;
        ss << tmp;
        ss >> *x;
    }
    else
    {
       if (!_isNumber(*x))
           _msg_exit("configuration file error, client_max_body_size");
        tmp = atoi(x->c_str());
    }
    if (tmp > (int)10485760000)
        _msg_exit("configuration file error, client_max_body_size");
}

int     Utils::_isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
        if (!_ft_isdigit(str[i]))
            return 0;
    return 1;
}

int	    Utils::_ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

void    Utils::_msg_exit(std::string s)
{
    std::cerr << s << std::endl;
    exit(1);
}

int     Utils::_ft_isprint(int c)
{
	if (c > 32 && c < 127)
		return (1);
	else
		return (0);
}

/* 
* These ones
* are for
* Response class:
*/

void    Utils::_setErrorPages()
{
    _error_page[404] = "errors/404.html";
    _error_page[405] = "errors/405.html";
    _error_page[413] = "errors/413.html";
}

std::string Utils::_getErrorPage(std::string *type)
{
    std::string content;
    std::ifstream f("./www/" + _error_page[_errorCode]);
    if (f.good())
    {
        std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        content = str;
        *type = "html";
    }
    f.close();
    return content;
}

std::string Utils::_getFile(std::ifstream *f)
{
    std::string str((std::istreambuf_iterator<char>(*f)), std::istreambuf_iterator<char>());
    std::string content = str;
    _errorCode = 200;
    return content;
}

std::string Utils::_getDefaultFile(std::string *type)
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


std::string     Utils::_getContentLength(int size)
{
    std::ostringstream oss;

    oss << "Content-Length: ";
    oss << size << "\r\n";
    return oss.str();
}

std::string     Utils::_getCacheControl()
{
    std::string str;

    str = "Cache-Control: no-cache, private\r\n";
    return str;
}

std::string     Utils::_getLocation(std::string url)
{
    std::ostringstream oss;

    oss << "Location: ";
    oss << url << "\r\n";
    return oss.str();
}

std::string     Utils::_getStatus(int err_code)
{
    if (err_code == 100)
        return " Continue\r\n";
    else if (err_code == 101)
        return " Switching Protocols\r\n";
    else if (err_code == 103)
        return " Early Hints\r\n";

    else if (err_code == 200)
        return " OK\r\n";
    else if (err_code == 201)
        return " Created\r\n";
    else if (err_code == 202)
        return " Accepted\r\n";
    else if (err_code == 203)
        return " Non-Authoritative Information\r\n";
    else if (err_code == 204)
        return " No Content\r\n";
    else if (err_code == 205)
        return " Reset Content\r\n";
    else if (err_code == 206)
        return " Partial Content\r\n";

    else if (err_code == 300)
        return " Multiple Choice\r\n";
    else if (err_code == 301)
        return " Moved Permanently\r\n";
    else if (err_code == 302)
        return " Found\r\n";
    else if (err_code == 303)
        return " See Other\r\n";
    else if (err_code == 304)
        return " Not Modified\r\n";
    else if (err_code == 307)
        return " Temporary Redirect\r\n";
    else if (err_code == 308)
        return " Permanent Redirect\r\n";

    else if (err_code == 400)
        return " Bad Request\r\n";
    else if (err_code == 401)
        return " Unauthorized\r\n";
    else if (err_code == 402)
        return " Payment Required\r\n";
    else if (err_code == 403)
        return " Forbidden\r\n";
    else if (err_code == 405)
        return " Method Not Allowed\r\n";
    else if (err_code == 406)
        return " Not Acceptable\r\n";
    else if (err_code == 407)
        return " Proxy Authentication Required\r\n";
    else if (err_code == 408)
        return " Request Timeout\r\n"; 
    else if (err_code == 409)
        return " Conflict\r\n";
    else if (err_code == 410)
        return " Gone\r\n";
    else if (err_code == 411)
        return " Length Required\r\n";
    else if (err_code == 412)
        return " Precondition Failed\r\n";
    else if (err_code == 413)
        return " Payload Too Large\r\n";
    else if (err_code == 414)
        return " URI Too Long\r\n";
    else if (err_code == 415)
        return " Unsupported Media Type\r\n";
    else if (err_code == 416)
        return " Range Not Satisfiable\r\n";
    else if (err_code == 417)
        return " Expectation Failed\r\n";
    else if (err_code == 418)
        return " I'm a teapot\r\n";
    else if (err_code == 425)
        return " Too Early\r\n";
    else if (err_code == 426)
        return " Upgrade Required\r\n";
    else if (err_code == 428)
        return " Precondition Required\r\n";
    else if (err_code == 429)
        return " Too Many Request\r\n";
    else if (err_code == 431)
        return " Request Header Fields Too Large\r\n";
    else if (err_code == 451)
        return " Unavailable For Legal Reasons\r\n";

    else if (err_code == 500)
        return " Internal Server Error\r\n";
    else if (err_code == 501)
        return " Not Implemented\r\n";
    else if (err_code == 502)
        return " Bad Gateway\r\n";
    else if (err_code == 503)
        return " Service Unavailable\r\n";
    else if (err_code == 504)
        return " Gateway Timeout\r\n";
    else if (err_code == 505)
        return " HTTP Version Not Supported\r\n";
    else if (err_code == 506)
        return " Variant Also Negotiates\r\n";
    else if (err_code == 510)
        return " Not Extended\r\n";
    else if (err_code == 511)
        return " Network Authentication Required\r\n";

    else
        return " Not Found\r\n";
}

std::string     Utils::_getContentType(std::string type)
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

int     Utils::_sendall(int clientSocket, const char *buf, int *size)
{
    int total = 0;
    int bytesleft = *size;
    int n;

    while (total < *size)
    {
        n = send(clientSocket, buf, bytesleft, 0);
        if (n == -1)
            break ;
        total += n;
        bytesleft -= n;
    }
    *size = total;
    return n == -1 ? -1 : 0;
}