#include "Utils.hpp"

Utils::Utils() {}

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
    std::cerr << RED << s << RESET << std::endl;
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

std::string Utils::_getErrorPage(std::string *type)
{
    std::ostringstream oss;

    oss <<  "<!DOCTYPE html>"
            "<html>"
                "<style>"
                    "h1 {text-align: center;}"
                    "p {text-align: center;}"
                "</style>"
                "<body>"
                    "<h1>" << _errorCode << " " << _getStatus(_errorCode) << "</h1>"
                    "<hr>"
                    "<p><small>Webserv</small></p>"
                "</body>"
            "</html>";
    *type = "html";
    return oss.str();
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
    if (err_code == 200)
        return " OK\r\n";
    else if (err_code == 201)
        return " Created\r\n";
    else if (err_code == 202)
        return " Accepted\r\n";
    else if (err_code == 204)
        return " No Content\r\n";

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
    else if (err_code == 403)
        return " Forbidden\r\n";
    else if (err_code == 405)
        return " Method Not Allowed\r\n";
    else if (err_code == 406)
        return " Not Acceptable\r\n";
    else if (err_code == 413)
        return " Payload Too Large\r\n";
    else if (err_code == 414)
        return " URI Too Long\r\n";

    else if (err_code == 502)
        return " Bad Gateway\r\n";
    else if (err_code == 505)
        return " HTTP Version Not Supported\r\n";

    else
        return " Not Found\r\n";
}

std::string     Utils::_getContentType(std::string type)
{
    std::string str;

    str = "Content-Type: ";
    if (type == "html" || type == "/")
        str += "text/html";
    else if (type == "css")
        str += "text/css";
    else if (type == "csv")
        str += "text/csv";
    else if (type == "js")
        str += "text/javascript";

    else if (type == "jpeg" || type == "jpg")
        str += "image/jpeg";
    else if (type == "gif")
        str += "image/gif";
    else if (type == "png")
        str += "image/png";
    else if (type == "bmp")
        str += "image/bmp";
    else
        str += "text/plain";
    str += "; charset=UTF-8\r\n";
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

std::string Utils::_getFileString(std::string path)
{
    std::ifstream infile;
    std::stringstream strStream;

    infile.open(path);
    strStream << infile.rdbuf();
    return strStream.str();
}

std::string Utils::_getAutoindexHtml(std::string path, std::string uri, std::string *type)
{
    *type = "html";
    std::string templateContent = _getFileString("./assets/autoindex_template.html"); //"./Networking/Utils/autoindex_template.html");
    std::string linkPrefix = (uri[uri.size() - 1] == '/' ? uri : uri + "/");
    std::string fileList;
    DIR *dirp = opendir(path.c_str());
    struct dirent *direntp = readdir(dirp);
    for (; direntp != NULL; direntp = readdir(dirp))
    {
        if (std::string(direntp->d_name) == ".." || std::string(direntp->d_name) == ".")
            continue;
        fileList +=
            "<a href=\"" + linkPrefix + direntp->d_name + "\">" + direntp->d_name + "</a>\n";
    }
    closedir(dirp);
    templateContent.replace(templateContent.find("$2"), 2, fileList);
    return templateContent;
}

// bool Utils::_shouldBeEscaped(char c)
// {
//     std::string unescapedChars = UNESCAPED_CHARS;
//     if (unescapedChars.find(c) != std::string::npos ||
//         (c >= 'a' && c <= 'z') ||
//         (c >= 'A' && c <= 'Z') ||
//         (c >= '0' && c <= '9'))
//             return false;
//     return true;
// }

// std::string Utils::_getEscapedChar(char c)
// {
//     std::string result = "%";
//     std::stringstream stream;
//     stream << std::hex << std::setfill('0') << std::setw(2) << (int)c;
//     result += stream.str();
//     for (unsigned i = 0; i < result.size(); i++)
//         result[i] = std::toupper(result[i]);
//     return result;
// }

// std::string Utils::_encodeURIComponent(std::string src)
// {
//     std::string result;

//     for (unsigned i = 0; i < src.size(); i++)
//     {
//         if (_shouldBeEscaped(src[i]))
//             result += _getEscapedChar(src[i]);
//         else
//             result += src[i];
//     }
//     return result;
// }

std::string Utils::_decodeURIComponent(std::string encodedURI)
{
    std::string decodedURI;

    for (unsigned i = 0; i < encodedURI.size(); i++)
    {
        if (encodedURI[i] == '+')
            decodedURI += ' ';
        else if (encodedURI[i] == '%')
        {
            unsigned int c;
            std::stringstream ss;
            ss << std::hex << std::string(encodedURI, i + 1, 2);
            ss >> c;
            decodedURI += static_cast<char>(c);
            i += 2;
        }
        else
            decodedURI += encodedURI[i];
    }
    return decodedURI;
}