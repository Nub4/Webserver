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

void    Parse::readFile(char *conf, std::string path)
{
    std::string line;
    std::string filename = conf;
    std::string str = path + "/confs/" + filename;
    std::ifstream infile;
    int pos;

    infile.open(str);
    if (!infile.is_open())
        _msg_exit("Error: cannot open the file");
    while (getline(infile, line))
    {
        pos = line.find('#', 0);
        if (pos != -1)
            line.erase(pos, line.size());
        _conf_file += line;
        _conf_file += "\n";
    }
    infile.close();
}

void    Parse::readBinaryFile(char *conf, std::string path)
{
    std::string filename = conf;
    std::string str = path + "/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> _binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
}

void    Parse::getConfigurationData()
{
    size_t pos = 0;
    while (pos != _conf_file.size() -1)
        pos = _parseServer(pos);
    _checkServerValues();
    _erase_separator();
}

int     Parse::_parseLocation(int start_pos, std::string temp, struct locationBlock *loct)
{
    size_t i;
    // find where location block starts
    int pos = temp.find("location", start_pos);
    // position after location word
    int pos_after_location = pos + strlen("location");
    // check if location name is correct
    for (i = pos_after_location; temp[i] != '/'; i++)
        if (_ft_isprint(temp[i]))
            _msg_exit("configuration file error, location");
    // find position after location name and insert location name
    for (; _ft_isprint(temp[i]); i++)
        loct->name.push_back(temp[i]);
    size_t pos_bracket;
    if (temp[i - 1] == '{')
    {
        loct->name.pop_back();
        pos_bracket = i - 1;
    }
    else
    {
        // find position of first bracket
        pos_bracket = temp.find("{", i);
        // check if errors before first bracket
        for (; i < pos_bracket; i++)
            if (_ft_isprint(temp[i]))
                _msg_exit("configuration file error, location3");
    }
    // check ending bracket
    int location_end_pos = _checkClosingBracket(pos_bracket + 1, temp);
    // get data from location block
    _get_location(pos_bracket + 1, location_end_pos - 1, temp, loct);
    return location_end_pos;
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
    int server_end_pos = _checkClosingBracket(pos_bracket + 1, _conf_file);
    // get data from server block
    _get_conf(pos_bracket + 1, server_end_pos - 1);
    return server_end_pos;
}

void    Parse::_get_location(int start, int end, std::string temp, struct locationBlock *loct)
{
    temp.erase(0, start);
    temp.erase(end - start, temp.size() - (end - start));
    std::istringstream iss(temp);
    std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::vector<std::string> s;

    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
    {
        if (_is_validLocationName(*it))
        {
            s.clear();
            it++;
            if (*(it - 1) == "autoindex")
                loct->autoindex = *it;
            else if (*(it - 1) == "methods")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    loct->methods.push_back(*it);
                loct->methods.push_back(*it);
            }
            else if (*(it - 1) == "index")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    loct->index.push_back(*it);
                loct->index.push_back(*it);
            }
        }
        else
            _msg_exit("configuration file error, location");
    }
}

void    Parse::_get_conf(int start, int end)
{
    int pos = 0;
    struct serverBlock serv;
    struct locationBlock loct;
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
            memset(&loct, 0, sizeof(loct));
            s.clear();
            it++;
            if (*(it - 1) == "listen")
                serv.listen = *it;
            else if (*(it - 1) == "server_name")
                serv.server_name = *it;
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
                serv.autoindex = *it;
            else if (*(it - 1) == "methods")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    serv.methods.push_back(*it);
                serv.methods.push_back(*it);
            }
            else if (*(it - 1) == "location")
            {
                pos = _parseLocation(pos, temp, &loct);
                for (; it != words.end(); it++)
                    if (*it == "}" || it->back() == '}')
                        break;
                serv.location.push_back(loct);
           }
        }
        else
            _msg_exit("configuration file error");
    }
    _serverContent.push_back(serv);
}

void    Parse::_checkServerValues()
{
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        if (!it->listen.empty() && it->listen.back() != ';')
            _msg_exit("configuration file error, listen");
        if (!it->server_name.empty() && it->server_name.back() != ';')
            _msg_exit("configuration file error, server_name");
        if (!it->autoindex.empty() && it->autoindex != "on;" && it->autoindex != "off;")
            _msg_exit("configuration file error, autoindex");
        if (!it->methods.empty())
        {
            std::vector<std::string>::iterator it2 = it->methods.begin();
            for (; it2 != it->methods.end() - 1; it2++)
                if (*it2 != "GET" && *it2 != "POST" && *it2 != "DELETE")
                    _msg_exit("configuration file error, methods");
            if (*it2 != "GET;" && *it2 != "POST;" && *it2 != "DELETE;")
                _msg_exit("configuration file error, methods");
        }
        if (!it->error_page.empty() && it->error_page.begin()->first != 404 && it->error_page.begin()->second.back() != ';')
            _msg_exit("configuration file error, error_page");
        if (!it->location.empty())
        {
            for (std::vector<locationBlock>::iterator it4 = it->location.begin(); it4 != it->location.end(); it4++)
            {
                if (!it4->autoindex.empty() && it4->autoindex != "on;" && it4->autoindex != "off;")
                    _msg_exit("configuration file error, autoindex");
                if (!it4->index.empty())
                {
                    std::vector<std::string>::iterator it5 = it4->index.end() - 1;
                    if (it5->back() != ';')
                        _msg_exit("configuration file error, index");
                }
                if (!it4->methods.empty())
                {
                    std::vector<std::string>::iterator it6 = it4->methods.begin();
                    for (; it6 != it4->methods.end() - 1; it6++)
                        if (*it6 != "GET" && *it6 != "POST" && *it6 != "DELETE")
                            _msg_exit("configuration file error, methods");
                    if (*it6 != "GET;" && *it6 != "POST;" && *it6 != "DELETE;")
                        _msg_exit("configuration file error, methods");
                }
            }
        }
    }
}

void    Parse::_erase_separator()
{
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        if (!it->listen.empty())
            it->listen.pop_back();
        if (!it->server_name.empty())
            it->server_name.pop_back();
        if (!it->autoindex.empty())
            it->autoindex.pop_back();
        if (!it->methods.empty())
        {
            std::vector<std::string>::iterator it2 = it->methods.end() - 1;
            it2->pop_back();
        }
        if (!it->error_page.empty())
        {
            std::map<int, std::string>::reverse_iterator it3 = it->error_page.rbegin();
            it3->second.pop_back();
        }
        if (!it->location.empty())
        {
            for (std::vector<locationBlock>::iterator it4 = it->location.begin(); it4 != it->location.end(); it4++)
            {
                if (it4->name.back() == '{')
                    it4->name.pop_back();
                if (!it4->autoindex.empty())
                    it4->autoindex.pop_back();
                if (!it4->index.empty())
                {
                    std::vector<std::string>::iterator it5 = it4->index.end() - 1;
                    it5->pop_back();
                }
                if (!it4->methods.empty())
                {
                    std::vector<std::string>::iterator it6 = it4->methods.end() - 1;
                    it6->pop_back();
                }
            }
        }
    }
}

void    Parse::printStructs()
{
    // Check if everything is correctly parsed
    int count = 1;
    std::cout << std::endl;
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        std::cout << "--- " << count << " server block ---\n\n";
        if (!it->listen.empty())
            std::cout << "listen:      " << it->listen << std::endl;
        if (!it->server_name.empty())
            std::cout << "server_name: " << it->server_name << std::endl;
        if (!it->autoindex.empty())
            std::cout << "autoindex:   " << it->autoindex << std::endl;
        if (!it->methods.empty())
        {
            std::cout << "methods:     ";
            for (std::vector<std::string>::iterator it2 = it->methods.begin(); it2 != it->methods.end(); it2++)
                std::cout << *it2 << " ";
            std::cout << std::endl;
        }
        if (!it->error_page.empty())
        {
            std::cout << "error_page:  ";
            for (std::map<int, std::string>::iterator it3 = it->error_page.begin(); it3 != it->error_page.end(); it3++)
                std::cout << it3->first << " " << it3->second << std::endl;
        }
        if (!it->location.empty())
        {
            for (std::vector<locationBlock>::iterator it4 = it->location.begin(); it4 != it->location.end(); it4++)
            {
                std::cout << "location ";
                if (!it4->name.empty())
                    std::cout << it4->name << " :" << std::endl;
                if (!it4->autoindex.empty())
                    std::cout << "  autoindex: " << it4->autoindex << std::endl;
                if (!it4->methods.empty())
                {
                    std::cout << "  methods:   ";
                    for (std::vector<std::string>::iterator it5 = it4->methods.begin(); it5 != it4->methods.end(); it5++)
                        std::cout << *it5 << " ";
                    std::cout << std::endl;
                }
                if (!it4->index.empty())
                {
                    std::cout << "  index:     ";
                    for (std::vector<std::string>::iterator it6 = it4->index.begin(); it6 != it4->index.end(); it6++)
                        std::cout << *it6 << " ";
                    std::cout << std::endl;
                }
            }
        }
        count++;
        std::cout << std::endl;
    }
}

std::vector<Parse::serverBlock>    Parse::getServerContent() { return _serverContent; }

int     Parse::_checkClosingBracket(int pos, std::string s)
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

bool    Parse::_is_validLocationName(std::string name)
{
    for (std::vector<std::string>::iterator it = _location_names.begin(); it != _location_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

void    Parse::_msg_exit(std::string s)
{
    std::cerr << s << std::endl;
    exit(1);
}