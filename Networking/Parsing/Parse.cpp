#include "Parse.hpp"

// Default constructor initialize valid server names
// and location names for configuration file
Parse::Parse()
{
    _server_names.push_back("listen");
    _server_names.push_back("server_name");
    _server_names.push_back("error_page");
    _server_names.push_back("autoindex");
    _server_names.push_back("method");
    _server_names.push_back("location");
    _server_names.push_back("client_max_body_size");
    _server_names.push_back("root");

    _location_names.push_back("index");
    _location_names.push_back("autoindex");
    _location_names.push_back("method");
    _location_names.push_back("root");
    _location_names.push_back("upload_enable");
    _location_names.push_back("upload_path");
    _location_names.push_back("cgi_extension");
    _location_names.push_back("cgi_path");
    _location_names.push_back("client_max_body_size");
}

// Read given text file and remove context from the line
// after command mark '#'
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

// Read binary files
void    Parse::readBinaryFile(char *conf, std::string path)
{
    std::string filename = conf;
    std::string str = path + "/confs/" + filename;
    std::ifstream infile(str, std::ios::binary);
    std::vector<unsigned char> _binary_file((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
}

// Start parsing your configuration file
void    Parse::getConfigurationData()
{
    size_t pos = 0;
    while (pos != _conf_file.size() -1)
        pos = _parseServer(pos);
    _checkServerValues();
    _check_same_host();
}

// Start parsing your location block data,
// here you find positions of location block and check if the form is valid.
// Returning end postion of location block
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

// Start parsing your server block data,
// here you find positions of server block and check if the form is valid.
// Returning end postion of server block
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

// Check if given names are correct in location block.
// Insert data to your location block vector.
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
            else if (*(it - 1) == "upload_enable")
                loct->upload_enable = *it;
            else if (*(it - 1) == "upload_path")
                loct->upload_path = *it;
            else if (*(it - 1) == "cgi_extension")
                loct->cgi_extension = *it;
            else if (*(it - 1) == "cgi_path")
                loct->cgi_path = *it;
            else if (*(it - 1) == "client_max_body_size")
                loct->client_max_body_size = *it;
        }
        else
            _msg_exit("configuration file error, location");
    }
}

// Inserting error_page values.
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

// Check if given names are correct in server block.
// Insert data to your serverContent vector.
void    Parse::_get_conf(int start, int end)
{
    size_t pos = 0;
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
                _insert_error_page(words, *it, &serv);
                it++;
            }
            else if (*(it - 1) == "autoindex")
                serv.autoindex = *it;
            else if (*(it - 1) == "method")
            {
                for (; it->back() != ';' && it != words.end(); it++)
                    serv.method.push_back(*it);
                serv.method.push_back(*it);
            }
            else if (*(it - 1) == "location")
            {
                pos = _parseLocation(pos, temp, &loct);
                for (; it != words.end(); it++)
                    if (*it == "}" || it->back() == '}')
                        break;
                serv.location.push_back(loct);
            }
            else if (*(it - 1) == "root")
                serv.root = *it;
            else if (*(it - 1) == "client_max_body_size")
                serv.client_max_body_size = *it;
        }
        else
            _msg_exit("configuration file error");
    }
    _serverContent.push_back(serv);
}

// Checks if word ends to ';' separator and
// after that erase the separator 
void    Parse::_checkBackChar(std::string *x, std::string name)
{
    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, " + name);
    x->pop_back();
}

// Checks if method is correct, valid methods
// are GET POST and DELETE.
void    Parse::_check_method(std::vector<std::string> *v)
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

// Checks if autoindex is correct,
// on and off are the two options to use
void    Parse::_check_autoindex(std::string *x)
{
    if (*x != "on;" && *x != "off;")
        _msg_exit("configuration file error, autoindex");
    x->pop_back();
}

// Checks if error_page is correct
// and erase the separator ,404 is valid error code 
void    Parse::_check_error_page(std::map<int, std::string> *m)
{
    std::map<int, std::string>::iterator it = m->begin();
    if ((it->first != 404 && it->second.back() != ';') || std::count(it->second.begin(), it->second.end(), ';') != 1)
        _msg_exit("configuration file error, error_page");
    it->second.pop_back();
}

// Checks if listen is correct
// and erase the separator.
void    Parse::_check_listen(std::string *x)
{
    if (std::count(x->begin(), x->end(), ';') != 1 || x->back() != ';')
        _msg_exit("configuration file error, listen");
    x->pop_back();
    if (!_isNumber(*x) || atoi(x->c_str()) < 0 || atoi(x->c_str()) > 65535)
        _msg_exit("configuration file error, listen");
}

// This functions checks if everything is correct in
// our configuration file and erase the separator end
// of the last word.
void    Parse::_checkServerValues()
{
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        if (!it->listen.empty())
            _check_listen(&it->listen);
        if (!it->server_name.empty())
            _checkBackChar(&it->server_name, "server_name");
        if (!it->autoindex.empty())
            _check_autoindex(&it->autoindex);
        if (!it->method.empty())
            _check_method(&it->method);
        if (!it->error_page.empty())
            _check_error_page(&it->error_page);
        if (!it->root.empty())
            _checkBackChar(&it->root, "root");
        if (!it->client_max_body_size.empty())
           _checkBackChar(&it->root, "client_max_body_size");
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
                if (!it4->upload_enable.empty())
                    _checkBackChar(&it4->upload_enable, "upload_enable");
                if (!it4->upload_path.empty())
                    _checkBackChar(&it4->upload_path, "upload_path");
                if (!it4->cgi_extension.empty())
                    _checkBackChar(&it4->cgi_extension, "cgi_extension");
                if (!it4->cgi_path.empty())
                    _checkBackChar(&it4->cgi_path, "cgi_path");
                if (!it4->client_max_body_size.empty())
                    _checkBackChar(&it4->client_max_body_size, "client_max_body_size");
            }
        }
    }
}

// Checks if two servers are listening same port
// and having same ip address
void    Parse::_check_same_host()
{
    if (_serverContent.size() < 2)
        return ;
    for (size_t i = 0; i < _serverContent.size(); i++)
        for (size_t j = i + 1; j < _serverContent.size(); j++)
            if (_serverContent[i].listen == _serverContent[j].listen && _serverContent[i].server_name == _serverContent[j].server_name)
                _msg_exit("Error, two servers has same server_name and host");
}

// Returning  serverContent vector data.
std::vector<Parse::serverBlock>    Parse::getServerContent() { return _serverContent; }

// This functions tells if there is a closing bracket or 
// is the block left open.
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

// Tells if character is pritable or not
int     Parse::_ft_isprint(int c)
{
	if (c > 32 && c < 127)
		return (1);
	else
		return (0);
}

// Tells if string is number or not
int     Parse::_isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
        if (!_ft_isdigit(str[i]))
            return 0;
    return 1;
}

// Tells if character is number or not
int	    Parse::_ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// Returning true if server name is valid
bool    Parse::_is_validName(std::string name)
{
    for (std::vector<std::string>::iterator it = _server_names.begin(); it != _server_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

// Returning true if location name is valid
bool    Parse::_is_validLocationName(std::string name)
{
    for (std::vector<std::string>::iterator it = _location_names.begin(); it != _location_names.end(); it++)
        if (name == *it)
            return true;
    return false;
}

// Prints the wanted error message and exit from the program
void    Parse::_msg_exit(std::string s)
{
    std::cerr << s << std::endl;
    exit(1);
}

// Prints all the data from the configuration file,
// so we can see parsing is done correctly.
void    Parse::printStructs()
{
    int count = 1;
    std::cout << std::endl;
    for (std::vector<serverBlock>::iterator it = _serverContent.begin(); it != _serverContent.end(); it++)
    {
        std::cout << "--- " << count << " server block ---\n\n";
        if (!it->listen.empty())
            std::cout << "listen:               " << it->listen << std::endl;
        if (!it->server_name.empty())
            std::cout << "server_name:          " << it->server_name << std::endl;
        if (!it->autoindex.empty())
            std::cout << "autoindex:            " << it->autoindex << std::endl;
        if (!it->method.empty())
        {
            std::cout << "method:               ";
            for (std::vector<std::string>::iterator it2 = it->method.begin(); it2 != it->method.end(); it2++)
                std::cout << *it2 << " ";
            std::cout << std::endl;
        }
        if (!it->error_page.empty())
        {
            std::cout << "error_page:           ";
            for (std::map<int, std::string>::iterator it3 = it->error_page.begin(); it3 != it->error_page.end(); it3++)
                std::cout << it3->first << " " << it3->second << std::endl;
        }
        if (!it->root.empty())
            std::cout << "root:                 " << it->root << std::endl;
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
                if (!it4->upload_enable.empty())
                    std::cout << "     upload_enable:        " << it4->upload_enable << std::endl;
                if (!it4->upload_path.empty())
                    std::cout << "     upload_path:          " << it4->upload_path << std::endl;
                if (!it4->cgi_extension.empty())
                    std::cout << "     cgi_extension:        " << it4->cgi_extension << std::endl;
                if (!it4->cgi_path.empty())
                    std::cout << "     cgi_path:             " << it4->cgi_path << std::endl;
                if (!it4->client_max_body_size.empty())
                    std::cout << "     client_max_body_size: " << it4->client_max_body_size << std::endl;
            }
        }
        count++;
        std::cout << std::endl;
    }
}