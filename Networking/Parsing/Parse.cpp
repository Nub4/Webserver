#include "Parse.hpp"

Parse::Parse()
{
    _valid_names.push_back("listen");
    _valid_names.push_back("server_name");
    _valid_names.push_back("root");
    _valid_names.push_back("error_page_404");
    _valid_names.push_back("client_body_size");
    _valid_names.push_back("methods");
    _valid_names.push_back("autoindex");
}

void    Parse::setFile(char *conf)
{
    std::string line;
    std::string filename = conf;
    std::string str = "/Users/minummin/Webserver/confs/" + filename;
    std::ifstream infile;

    infile.open(str);
    if (!infile.is_open())
        _error_manage("Error: cannot open the file");

    while (getline(infile, line))
    {
        _conf_file += line;
        _conf_file += "\n";
    }
    infile.close();
}

void    Parse::setBinaryFile(char *conf)
{
    std::string filename = conf;
    std::string str = "/Users/minummin/Webserver/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> _binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
}

bool    Parse::_is_validName(std::string name)
{
    for (std::vector<std::string>::iterator it = _valid_names.begin(); it != _valid_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

size_t  Parse::_serverLineCount(std::string str, std::string c, std::string z)
{
    std::istringstream f(str);
    int count = 0;
    int mark = 0;
    while (std::getline(f, str))
    {
        if (str == c)
        {
            while (std::getline(f, str))
            {
                if (str == z)
                {
                    mark = 1;
                    break ;
                }
                count++;
            }
        }
        if (mark == 1)
            break ;
    }
    if (mark == 0)
        return 0;
    return count;
}

int     Parse::_countWord(std::string s, std::string word)
{
    std::istringstream f(s);
    std::string line;
    int n = 0;

    while (std::getline(f, line))
        if (line == word)
            n++;
    return n;
}

void    Parse::setConfs()
{
    std::string temp = _conf_file;
    std::istringstream f(temp);
    std::string line;
    std::string val;
    std::string key;
    std::vector<std::string> s;
    int count;
    int n = _countWord(temp, "server");

    while (n > 0)
    {
        count = _serverLineCount(temp, "{", "}");
        while (std::getline(f, line))
            if (line == "server")
                break ;
        temp.erase(0, strlen(line.c_str()));
        std::getline(f, line);
        temp.erase(0, strlen(line.c_str()));
        if (line == "{")
        {
            while (std::getline(f, line) && count > 0)
            {
                temp.erase(0, strlen(line.c_str()));
                s.clear();
                std::istringstream ss(line);
                ss >> key;
                if (_is_validName(key) == true)
                {
                    while (ss >> val)
                        s.push_back(val);
                    _serverContent.insert(std::pair<std::string, std::vector<std::string> >(key, s));
                }
                else
                    std::cout << key << " is invalid setup name in config file\n";
                count--;
            }
            temp.erase(0, strlen(line.c_str()));
        }
        _serverBlocks.push_back(_serverContent);
        _serverContent.clear();
        n--;
    }

    for (std::vector<std::map<std::string, std::vector<std::string> > >::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); it++)
    {
        for (std::map<std::string, std::vector<std::string> >::iterator it2 = it->begin(); it2 != it->end(); it2++)
        {
            std::cout << it2->first << " ";
            for (std::vector<std::string>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++)
                std::cout << *it3 << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void    Parse::_error_manage(std::string str)
{
    std::cerr << str << std::endl;
    exit(1);
}