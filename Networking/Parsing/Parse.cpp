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

size_t  Parse::lineCount(std::string str)
{
    size_t i = -1;
    size_t count = 0;

    while (str[++i])
        if (str[i] == '\n')
            count++;
    return count;
}

bool    Parse::is_validName(std::string name)
{
    for (std::vector<std::string>::iterator it = _valid_names.begin(); it != _valid_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

void    Parse::setMap()
{
    std::istringstream f(_conf_file);
    std::string line;
    std::string val;
    std::string key;
    std::vector<std::string> s;
    
    while (std::getline(f, line))
    {
        s.clear();
        std::istringstream ss(line);
        ss >> key;
        if (is_validName(key) == true)
        {
            while (ss >> val)
                s.push_back(val);
            _conf_map.insert(std::pair<std::string, std::vector<std::string> >(key, s));
        }
        else
            std::cout << key << " is invalid setup name in config file\n";
    }
    // for (std::map<std::string, std::vector<std::string> >::iterator it = _conf_map.begin(); it != _conf_map.end(); it++)
    // {
    //     std::cout << it->first << " ";
    //     for (std::vector<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
    //         std::cout << *it2 << " ";
    //     std::cout << std::endl;
    // }
}

void    Parse::_error_manage(std::string str)
{
    std::cerr << str << std::endl;
    exit(1);
}