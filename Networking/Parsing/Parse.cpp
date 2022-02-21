#include "Parse.hpp"

Parse::Parse()
{
    _server_names.push_back("listen");
    _server_names.push_back("server_name");
    _server_names.push_back("error_page");
    _server_names.push_back("location");
    _server_names.push_back("client_max_body_size");

    _location_names.push_back("index");
    _location_names.push_back("autoindex");
    _location_names.push_back("method");
    _location_names.push_back("root");
    _location_names.push_back("return");
    _location_names.push_back("upload");
}

void    Parse::readConfFile(char *conf)
{
    std::string line;
    std::string filename = conf;
    std::string str = "./confs/" + filename;
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

void    Parse::getConfigurationData()
{
    size_t pos = 0;

    while (pos != _conf_file.size() -1)
        pos = _parseServer(pos);
    _checkServerValues();
    _check_same_host();
}

int     Parse::_parseLocation(int start_pos, std::string temp, struct locationBlock *loct)
{
    size_t i;
    size_t pos_bracket;
    int pos;
    int pos_after_location;
    int location_end_pos;

    pos = temp.find("location", start_pos);
    pos_after_location = pos + strlen("location");
    for (i = pos_after_location; temp[i] != '/'; i++)
        if (_ft_isprint(temp[i]))
            _msg_exit("configuration file error, location");
    for (; _ft_isprint(temp[i]); i++)
        loct->name.push_back(temp[i]);
    if (temp[i - 1] == '{')
    {
        loct->name.pop_back();
        pos_bracket = i - 1;
    }
    else
    {
        pos_bracket = temp.find("{", i);
        for (; i < pos_bracket; i++)
            if (_ft_isprint(temp[i]))
                _msg_exit("configuration file error, location3");
    }
    location_end_pos = _checkClosingBracket(pos_bracket + 1, temp);
    _get_location(pos_bracket + 1, location_end_pos - 1, temp, loct);
    return location_end_pos;
}

int     Parse::_parseServer(int start_pos)
{
    int pos;
    int pos_after_server;
    int pos_bracket;
    int server_end_pos;

    pos = _conf_file.find("server", start_pos);
    for (int i = start_pos; i < pos; i++)
        if (_ft_isprint(_conf_file[i]))
            _msg_exit("configuration file error");
    pos_after_server = pos + strlen("server");
    pos_bracket = _conf_file.find("{", pos_after_server);
    for (int i = pos_after_server; i < pos_bracket; i++)
        if (_ft_isprint(_conf_file[i]))
            _msg_exit("configuration file error");
    server_end_pos = _checkClosingBracket(pos_bracket + 1, _conf_file);
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
            else if (*(it - 1) == "method")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    loct->method.push_back(*it);
                loct->method.push_back(*it);
            }
            else if (*(it - 1) == "index")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    loct->index.push_back(*it);
                loct->index.push_back(*it);
            }
            else if (*(it - 1) == "root")
                loct->root = *it;
            else if (*(it - 1) == "return")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    loct->redirect.push_back(*it);
                loct->redirect.push_back(*it);
            }
            else if (*(it - 1) == "upload")
                loct->upload = *it;
        }
        else
            _msg_exit("configuration file error, location");
    }
}

void    Parse::_get_conf(int start, int end)
{
    size_t pos = 0;
    struct serverBlock serv;
    std::string temp = _conf_file;
    temp.erase(0, start);
    temp.erase(end - start, _conf_file.size() - (end - start));
    std::istringstream iss(temp);
    std::vector<std::string> words((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::vector<std::string> s;

    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
    {
        struct locationBlock loct;
        if (_is_validName(*it))
        {
            memset(&loct, 0, sizeof(loct));
            s.clear();
            it++;
            if (*(it - 1) == "listen")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    serv.listen.push_back(*it);
                serv.listen.push_back(*it);
            }
            else if (*(it - 1) == "server_name")
                serv.server_name = *it;
            else if (*(it - 1) == "error_page")
            {
                _insert_error_page(words, *it, &serv);
                it++;
            }
            else if (*(it - 1) == "location")
            {
                pos = _parseLocation(pos, temp, &loct);
                for (; it != words.end(); it++)
                    if (*it == "}" || it->back() == '}')
                        break;
                serv.location.push_back(loct);
            }
            else if (*(it - 1) == "client_max_body_size")
                serv.client_max_body_size = *it;
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
        if (!it->listen.empty())
            _check_listen(&it->listen);
        if (!it->server_name.empty())
            _checkBackChar(&it->server_name, "server_name");
        if (!it->error_page.empty())
            _check_error_page(&it->error_page);
        if (!it->client_max_body_size.empty())
            _check_body_size(&it->client_max_body_size);
        if (!it->location.empty())
        {
            for (std::vector<locationBlock>::iterator it4 = it->location.begin(); it4 != it->location.end(); it4++)
            {
                if (!it4->autoindex.empty())
                    _check_autoindex(&it4->autoindex);
                if (!it4->index.empty())
                    _checkBackChar(&(*it4->index.rbegin()), "index");
                if (!it4->method.empty())
                    _check_method(&it4->method);
                if (!it4->root.empty())
                    _checkBackChar(&it4->root, "root");
                if (!it4->redirect.empty())
                    _check_redirect(&it4->redirect);
                if (!it4->upload.empty())
                    _checkBackChar(&it4->upload, "upload");
            }
        }
    }
}

void    Parse::_insert_error_page(std::vector<std::string> words, std::string x, struct serverBlock *serv)
{
    std::vector<std::string> s;
    std::vector<std::string>::iterator it = words.begin();

    for (; it != words.end(); it++)
        if (*it == x)
            break;
    for (; it->back() != ';' && it != words.end(); it++)
        s.push_back(*it);
    if (s.size() != 1 || !_isNumber(s[0]))
        _msg_exit("configuration file error, error_page");
    s.push_back(*it);
    serv->error_page.insert(std::pair<int, std::string>(atoi(s[0].c_str()), s[1]));
}

void    Parse::_check_same_host()
{
    for (size_t i = 0; i < _serverContent.size(); i++)
    {
        if (_serverContent[i].listen.empty())
        {
            _serverContent[i].listen.push_back("8080");
            _serverContent[i].listen.push_back("127.0.0.1");
        }
    }
    if (_serverContent.size() < 2)
        return ;
    for (size_t i = 0; i < _serverContent.size(); i++)
        for (size_t j = i + 1; j < _serverContent.size(); j++)
            if (_serverContent[i].listen[0] == _serverContent[j].listen[0] && _serverContent[i].listen[1] == _serverContent[j].listen[1])
                _msg_exit("Error, two servers has same port and host");
}

std::vector<Parse::serverBlock>    Parse::getServerContent() { return _serverContent; }

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

void    Parse::printStructs()
{
    int count = 1;

    std::cout << std::endl;
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        std::cout << "--- " << count << " server block ---\n\n";
        if (!it->listen.empty() && it->listen.size() == 2)
            std::cout << "listen:               " << it->listen[0] << " " << it->listen[1] << std::endl;
        if (!it->listen.empty() && it->listen.size() == 1)
            std::cout << "listen:               " << it->listen[0] << std::endl;
        if (!it->server_name.empty())
            std::cout << "server_name:          " << it->server_name << std::endl;
        if (!it->error_page.empty())
        {
            std::cout << "error_page:           ";
            for (std::map<int, std::string>::iterator it3 = it->error_page.begin(); it3 != it->error_page.end(); it3++)
                std::cout << it3->first << " " << it3->second << std::endl;
        }
        if (!it->client_max_body_size.empty())
            std::cout << "client_max_body_size: " << it->client_max_body_size << std::endl;
        if (!it->location.empty())
        {
            for (std::vector<locationBlock>::iterator it4 = it->location.begin(); it4 != it->location.end(); it4++)
            {
                std::cout << "location ";
                if (!it4->name.empty())
                    std::cout << it4->name << " :" << std::endl;
                if (!it4->autoindex.empty())
                    std::cout << "     autoindex:            " << it4->autoindex << std::endl;
                if (!it4->method.empty())
                {
                    std::cout << "     method:               ";
                    for (std::vector<std::string>::iterator it5 = it4->method.begin(); it5 != it4->method.end(); it5++)
                        std::cout << *it5 << " ";
                    std::cout << std::endl;
                }
                if (!it4->index.empty())
                {
                    std::cout << "     index:                ";
                    for (std::vector<std::string>::iterator it6 = it4->index.begin(); it6 != it4->index.end(); it6++)
                        std::cout << *it6 << " ";
                    std::cout << std::endl;
                }
                if (!it4->root.empty())
                    std::cout << "     root:                 " << it4->root << std::endl;
                if (!it4->redirect.empty())
                {    
                    std::cout << "     return:               ";
                    for (std::vector<std::string>::iterator it7 = it4->redirect.begin(); it7 != it4->redirect.end(); it7++)
                        std::cout << *it7 << " ";
                    std::cout << std::endl;
                }
                if (!it4->upload.empty())
                    std::cout << "     upload:               " << it4->upload << std::endl;
            }
        }
        count++;
        std::cout << std::endl;
    }
}