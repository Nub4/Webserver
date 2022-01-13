#include "CGI.hpp"

CGI::CGI() {}

CGI::CGI(Parse::serverBlock server, std::vector<std::string> parsed)
{
    _env["SERVER_PROTOCOL"] = parsed[2];    // protocol
    _env["SERVER_PORT"] = server.listen[0]; 
    _env["REQUEST_METHOD"] = parsed[0];     // method
    _env["PATH_INFO"] = parsed[1];          // might be incorrect
    _env["PATH_TRANSLATED"] = parsed[1];    // might be incorrect
    // _env["SCRIPT_NAME"] = 
    // _env["QUERY_STRING"] = 
    // _env["REMOTE_HOST"] = 
    // _env["REMOTE_ADDR"] = 
    // _env["AUTH_TYPE"] = 
    // _env["REMOTE_USER"] = 
    // _env["REMOTE_IDENT"] = 
    // _env["CONTENT_TYPE"] = 
    // _env["CONTENT_LENGTH"] = 
    // _env["HTTP_ACCEPT_LANGUAGE"] = 
    _env["HTTP_USER_AGENT"] = _getUserAgent(parsed);
    // _env["HTTP_COOKIE"] = "";
    _env["SERVER_SOFTWARE"] = "WEBSERV/19.42";
    _env["SERVER_NAME"] = server.listen[1] + ":" + server.listen[0];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
        std::cout << it->first << " = " << it->second << std::endl;
}

CGI::~CGI() {}

std::string     CGI::_getUserAgent(std::vector<std::string> parsed)
{
    std::string str;
    
    for (std::vector<std::string>::iterator it = parsed.begin(); it != parsed.end(); it++)
        if (*(it - 1) == "User-Agent:")
            for (; *it != "Accept:"; it++)
                str += *it + " ";
    str.resize(str.size() - 1);
    return str;
}

void CGI::getEnv()
{
   

    const char *c_env[_env.size()];
    std::map<std::string, std::string>::iterator it = _env.begin();
    std::string tmp = it->first + '=' + it->second;
    c_env[0] = tmp.c_str();
    std::cout << c_env[0]<< std::endl;
}