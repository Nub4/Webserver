#include "CGI.hpp"

CGI::CGI() {}

CGI::CGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index)
{
    _env["SERVER_PROTOCOL"] = parsed[2];    // protocol
    _env["SERVER_PORT"] = server.listen[0]; 
    _env["REQUEST_METHOD"] = parsed[0];     // method
    _env["PATH_INFO"] = parsed[1];          // might be incorrect
    _env["PATH_TRANSLATED"] = parsed[1];    // might be incorrect
    char tmp[256];
	getcwd(tmp, 256);
	std::string curr_dir = tmp;
    _env["SCRIPT_NAME"] = curr_dir + "www/cgi-bin/" + index;
    _env["QUERY_STRING"] = ""; //might need to add something
    // _env["REMOTE_HOST"] = maybe add later
    // _env["REMOTE_ADDR"] = maybe add later
    // _env["AUTH_TYPE"] = maybe add
    // _env["REMOTE_USER"] = maybe add
    // _env["REMOTE_IDENT"] = maybe add
    // _env["CONTENT_TYPE"] = maybe add
    // _env["CONTENT_LENGTH"] = maybe later
    // _env["HTTP_ACCEPT_LANGUAGE"] = maybe later
    _env["HTTP_USER_AGENT"] = _getUserAgent(parsed);
    // _env["HTTP_COOKIE"] = "";
    _env["SERVER_SOFTWARE"] = "WEBSERV/19.42";
    _env["SERVER_NAME"] = server.listen[1] + ":" + server.listen[0];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    // for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    //     std::cout << it->first << " = " << it->second << std::endl;
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

void CGI::runCGI()
{
    const char *c_env[_env.size()];
	std::map<std::string, std::string>::iterator it = _env.begin();
	int i = 0;
    while (it != _env.end())
    {
		std::string tmp = it->first + '=' + it->second;
		c_env[i++] = tmp.c_str();
		it++;
	}

	int fd[2];
	pipe(fd);
	int pid = fork();
	if (pid > 0)
	{

	}
}