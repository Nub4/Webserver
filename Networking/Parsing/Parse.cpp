#include "Parse.hpp"

Parse::Parse()
{
    _server_names.push_back("listen");
    _server_names.push_back("server_name");
    _server_names.push_back("error_page");
    _server_names.push_back("autoindex");
    _server_names.push_back("methods");
    _server_names.push_back("location");

    _location_names.push_back("index");
    _location_names.push_back("autoindex");
    _location_names.push_back("methods");
}

void    Parse::readFile(char *conf)
{
    std::string line;
    std::string filename = conf;
    std::string str = "/Users/mikkonumminen/Webserv/confs/" + filename;
    std::ifstream infile;

    infile.open(str);
    if (!infile.is_open())
        _msg_exit("Error: cannot open the file");

    while (getline(infile, line))
    {
        _conf_file += line;
        _conf_file += "\n";
    }
    infile.close();
}

void    Parse::readBinaryFile(char *conf)
{
    std::string filename = conf;
    std::string str = "/Users/mikkonumminen/Webserv/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> _binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
}

void    Parse::getConfigurationData()
{
    size_t pos = 0;
    while (pos != _conf_file.size() -1)
        pos = _parseServer(pos);

    _erase_separator();
}

int     Parse::_parseServer(int start_pos)
{
    // find where server block starts
    int pos = _conf_file.find("server", start_pos);

    // check if errors before server block
    for (int i = start_pos; i < pos; i++)
        if (_ft_isprint(_conf_file[i]))
            _msg_exit("configuration file error");

    // check first bracket
    int pos_after_server = pos + strlen("server");
    int pos_bracket = _conf_file.find("{", pos_after_server);

    // check if errors before first bracket
    for (int i = pos_after_server; i < pos_bracket; i++)
        if (_ft_isprint(_conf_file[i]))
            _msg_exit("configuration file error");

    // check end position of block 
    int server_end_pos = _checkClosingBracket(pos_bracket + 1);
    
    // get data from server block
    _get_conf(pos_bracket + 1, server_end_pos - 1);

    return server_end_pos;
}

void    Parse::_get_conf(int start, int end)
{
    struct serverBlock serv;
    std::string temp = _conf_file;
    temp.erase(0, start);
    temp.erase(end - start, _conf_file.size() - (end - start));
    std::istringstream iss(temp);
    std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::vector<std::string> s;

    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
    {
        if (_is_validName(*it))
        {
            s.clear();
            it++;
            if (*(it - 1) == "listen")
            {
                if (it->back() != ';')
                    _msg_exit("configuration file error, listen");
                serv.listen = *it;
            }
            else if (*(it - 1) == "server_name")
            {
                if (it->back() != ';')
                    _msg_exit("configuration file error, server_name");
                serv.server_name = *it;
            }
            else if (*(it - 1) == "error_page")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    s.push_back(*it);
                if (s.size() != 1 || !_isNumber(s[0]))
                    _msg_exit("configuration file error, error_page");
                s.push_back(*it);
                serv.error_page.insert(std::pair<int, std::string>(atoi(s[0].c_str()), s[1]));
            }
            else if (*(it - 1) == "autoindex")
            {
                if (it->back() != ';')
                    _msg_exit("configuration file error, autoindex");
                serv.autoindex = *it;
            }
            else if (*(it - 1) == "methods")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                {
                    if (*it != "GET" && *it != "POST" && *it != "DELETE")
                        _msg_exit("configuration file error, methods");
                    serv.methods.push_back(*it);
                }
                if (*it != "GET;" && *it != "POST;" && *it != "DELETE;")
                    _msg_exit("configuration file error, methods");    
                serv.methods.push_back(*it);
            }
        }
        else
            _msg_exit("configuration file error");
    }
    _serverContent.push_back(serv);
}

void    Parse::_erase_separator()
{
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        it->listen.pop_back();
        it->server_name.pop_back();
        it->autoindex.pop_back();
        std::vector<std::string>::iterator it2 = it->methods.end() - 1;
        it2->pop_back();
        std::map<int, std::string>::reverse_iterator it3 = it->error_page.rbegin();
        it3->second.pop_back();
    }
}

void    Parse::printStructs()
{
    int count = 1;
    std::cout << std::endl;
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        std::cout << "--- " << count << " server block ---\n\n";
        std::cout << "listen:      " << it->listen << std::endl;
        std::cout << "server_name: " << it->server_name << std::endl;
        std::cout << "autoindex:   " << it->autoindex << std::endl;
        std::cout << "methods:     ";
        for (std::vector<std::string>::iterator it2 = it->methods.begin(); it2 != it->methods.end(); it2++)
            std::cout << *it2 << " ";
        std::cout << std::endl;
        std::cout << "error_page:  ";
        for (std::map<int, std::string>::iterator it3 = it->error_page.begin(); it3 != it->error_page.end(); it3++)
            std::cout << it3->first << " " << it3->second << std::endl;
        count++;

        std::cout << std::endl;
    }
}

int     Parse::_checkClosingBracket(int pos)
{
    int mark = 1;
    size_t i;

    for (i = pos; i < _conf_file.size() && mark != 0; i++)
    {
        if (_conf_file[i] == '{')
            mark++;
        else if (_conf_file[i] == '}')
            mark--;
    }
    if (i == _conf_file.size())
        return 0;
    return i;
}

int     Parse::_ft_isprint(int c)
{
	if (c > 32 && c < 127)
		return (1);
	else
		return (0);
}

int     Parse::_isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
        if (!_ft_isdigit(str[i]))
            return 0;
    return 1;
}

int	    Parse::_ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

bool    Parse::_is_validName(std::string name)
{
    for (std::vector<std::string>::iterator it = _server_names.begin(); it != _server_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

void    Parse::_msg_exit(std::string s)
{
    std::cerr << s << std::endl;
    exit(1);
}