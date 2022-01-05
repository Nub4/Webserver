#include "Utils.hpp"

Utils::Utils() {}

/* UTILS FOR PARSING CLASS */

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

void    Utils::_checkBackChar(std::string *x, std::string name)
{
    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, " + name);
    x->pop_back();
}

void    Utils::_check_listen(std::string *x)
{
    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, listen");
    x->pop_back();
    if (!_isNumber(*x) || atoi(x->c_str()) < 0 || atoi(x->c_str()) > 65535)
        _msg_exit("configuration file error, listen");
}

void    Utils::_check_error_page(std::map<int, std::string> *m)
{
    std::map<int, std::string>::iterator it = m->begin();
    if ((it->first != 404 && it->second.back() != ';') || std::count(it->second.begin(), it->second.end(), ';') != 1)
        _msg_exit("configuration file error, error_page");
    it->second.pop_back();
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

    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, client_max_body_size");
    x->pop_back();
    if (x->back() == 'M')
    {
        x->pop_back();
        tmp = atoi(x->c_str()) * 1048576;
        ss << tmp;
        ss >> *x;
    }
    else if (x->back() == 'K')
    {
        x->pop_back();
        tmp = atoi(x->c_str()) * 1024;
        ss << tmp;
        ss >> *x;
    }
    else
        tmp = atoi(x->c_str());
    if (!_isNumber(*x) || tmp > (int)10485760000)
        _msg_exit("configuration file error, client_max_body_size");
}

/* UTILS FOR EVERYWHERE */

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