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

void    Parse::setFile(char *conf)
{
    std::string line;
    std::string filename = conf;
    std::string str = "/Users/mikkonumminen/Webserv/confs/" + filename;
    std::ifstream infile;

    infile.open(str);
    if (!infile.is_open())
        msg_exit("Error: cannot open the file");

    while (getline(infile, line))
    {
        _conf_file += line;
        _conf_file += "\n";
    }
    infile.close();

    size_t pos = 0;
    while (pos != _conf_file.size() -1)
        pos = parseServer(pos);

    printStructs();
 //  exit(1);
}

void    Parse::msg_exit(std::string s)
{
    std::cerr << s << std::endl;
    exit(1);
}

int     Parse::parseServer(int start_pos)
{
    // find where server block starts
    int pos = _conf_file.find("server", start_pos);

    // check if errors before server block
    for (int i = start_pos; i < pos; i++)
        if (ft_isprint(_conf_file[i]))
            msg_exit("configuration file error");

    // check first bracket
    int pos_after_server = pos + strlen("server");
    int pos_bracket = _conf_file.find("{", pos_after_server);

    // check if errors before first bracket
    for (int i = pos_after_server; i < pos_bracket; i++)
        if (ft_isprint(_conf_file[i]))
            msg_exit("configuration file error");

    // check end position of block 
    int server_end_pos = checkClosingBracket(pos_bracket + 1);
    
    // get data from server block
    get_conf(pos_bracket + 1, server_end_pos - 1);

    return server_end_pos;
}

int     Parse::isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
        if (!ft_isdigit(str[i]))
            return 0;
    return 1;
}

int	    Parse::ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

void    Parse::get_conf(int start, int end)
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
            if (*it == "listen")
            {
                it++;
                if (it->back() == ';')
                    serv.listen = *it;
                else
                    msg_exit("configuration file error, listen");
            }
            if (*it == "server_name")
            {
                it++;
                if (it->back() == ';')
                    serv.server_name = *it;
                else
                    msg_exit("configuration file error, server_name");
            }
            if (*it == "error_page")
            {
                it++;
                while (it->back() != ';' && it != words.end())
                {
                    s.push_back(*it);
                    it++;
                }
                if (s.size() != 2 && !isNumber(s[0]))
                    msg_exit("configuration file error, error_page");
                s.push_back(*it);
                serv.error_page.insert(std::pair<int, std::string>(atoi(s[0].c_str()), s[1]));
            }
            if (*it == "autoindex")
            {
                it++;
                if (it->back() == ';')
                    serv.autoindex = *it;
                else
                    msg_exit("configuration file error, autoindex");
            }
            if (*it == "methods")
            {
                it++;
                while (it->back() != ';' && it != words.end())
                {
                    if (*it == "GET" || *it == "POST" || *it == "DELETE")
                        serv.methods.push_back(*it);
                    else
                        msg_exit("configuration file error, methods");
                    it++;
                }
                if (*it == "GET;" || *it == "POST;" || *it == "DELETE;")
                    serv.methods.push_back(*it);
                else
                    msg_exit("configuration file error, methods");
            }
            // if (*it == "location")
            // {
                
            // }
        }
        else
            msg_exit("configuration file error");
    }
    _serverContent.push_back(serv);
}

void    Parse::printStructs()
{
    int count = 1;
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        std::cout << count << " server block:\n";
        std::cout << "listen: " << it->listen << std::endl;
        std::cout << "server_name: " << it->server_name << std::endl;
        std::cout << "autoindex: " << it->autoindex << std::endl;
        std::cout << "methods: ";
        for (std::vector<std::string>::iterator it2 = it->methods.begin(); it2 != it->methods.end(); it2++)
            std::cout << *it2 << " ";
        std::cout << std::endl;
        std::cout << "error_page: ";
        for (std::map<int, std::string>::iterator it3 = it->error_page.begin(); it3 != it->error_page.end(); it3++)
            std::cout << it3->first << " " << it3->second << std::endl;
        count++;
        std::cout << std::endl;
    }
}

int     Parse::checkClosingBracket(int pos)
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

int     Parse::ft_isprint(int c)
{
	if (c > 32 && c < 127)
		return (1);
	else
		return (0);
}

void    Parse::setBinaryFile(char *conf)
{
    std::string filename = conf;
    std::string str = "/Users/mikkonumminen/Webserv/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> _binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
}

bool    Parse::_is_validName(std::string name)
{
    for (std::vector<std::string>::iterator it = _server_names.begin(); it != _server_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}