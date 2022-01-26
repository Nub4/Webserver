#include "CGI.hpp"

CGI::CGI() {}

CGI::CGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index)
{
	_initEnvCGI(server, parsed, index);
}

CGI::~CGI() {}

int CGI::runCGI()
{
	int status = 0;
	std::vector<std::string> tmp_env;
	std::map<std::string, std::string>::iterator it = _env.begin();
	for (; it != _env.end(); it++)
	{
		std::string tmp = it->first + '=' + it->second;
		tmp_env.push_back(tmp);
	}

	char *c_env[_env.size() + 1];
	for (unsigned long i = 0; i < _env.size(); i++)
		c_env[i] = (char *)tmp_env[i].c_str();
	c_env[_env.size()] = NULL;
/////////////////////////
	// for (int j = 0; c_env[j] != NULL; j++)
	// 	std::cout << c_env[j] << "\n";
/////////////////////////
	int fd_file = open("temp.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
	pid_t pid = fork();
	if (pid == 0)
	{
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
		char const *pathname = _env["PATH_TRANSLATED"].c_str();
		char **placeholder = NULL;
		if (execve(pathname, placeholder, c_env) == -1)
		{
			std::cout << "exec error\n";
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd_file);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}
	if (status == 127)
		return 1;
	return 0;
}

void CGI::_initEnvCGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index)
{
	std::map<std::string, std::string>	client_header(_parseClientVariables(parsed));	
	_parseClientToEnvVariables(client_header);

	_env["SERVER_PROTOCOL"] = parsed[2];
	_env["SERVER_NAME"] = server.listen[1];
	_env["SERVER_PORT"] = server.listen[0];
	_env["REQUEST_METHOD"] = parsed[0];
	_env["SCRIPT_NAME"] = _parseScriptName(index);
	_env["PATH_INFO"] = _parsePathInfo(index);
	_env["PATH_TRANSLATED"] = _parsePathTranslated();
	_env["QUERY_STRING"] = _parseQueryString(parsed[1]);
	_env["REMOTE_HOST"] = _parseRemoteHost(); 
	_env["REMOTE_ADDR"] = std::string();
	_env["AUTH_TYPE"] = std::string();
	_env["REMOTE_USER"] = std::string();
	_env["CONTENT_TYPE"] = _parseContentType();
	_env["CONTENT_LENGTH"] = _parseContentLength();
	_env["SERVER_SOFTWARE"] = "WEBSERV/42.19";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	// for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	// 	std::cout << it->first << " = " << it->second << std::endl;
}

std::map<std::string, std::string> CGI::_parseClientVariables(std::vector<std::string> parsed)
{
	std::map<std::string, std::string> client_header;
	std::vector<std::string>::iterator it = parsed.begin();
	while (it != parsed.end())
	{
		if (*(it->rbegin()) == ':')
		{
			std::string var = *it++;
			var.resize(var.size() - 1);
			if (it == parsed.end())
				break;
			std::string val;
			for (; it != parsed.end() && *(it->rbegin()) != ':'; it++)
				val = val + *it + " ";
			val.resize(val.size() - 1);
			client_header[var] = val;
		}
		else
			it++;
	}
	return client_header;
}

void CGI::_parseClientToEnvVariables(std::map<std::string, std::string> client_header)
{
	std::map<std::string, std::string>::iterator it = client_header.begin();
	for (; it != client_header.end(); it++)
	{
		std::string var = it->first;
		std::replace(var.begin(), var.end(), '-', '_');
		std::transform(var.begin(), var.end(), var.begin(), ::toupper);
		var = "HTTP_" + var;
		_env[var] = it->second;
	}
}

std::string CGI::_parseQueryString(std::string url)
{
	int pos = url.find("?");
	if (pos == -1)
		return std::string();
	return url.substr(pos + 1);
}

std::string CGI::_parseScriptName(std::string index)
{
	int pos = index.find(".py?");
	if (pos != -1)
		index = index.substr(0, pos + 3);
	else
	{
		pos = index.find(".py/");
		if (pos != -1)
			index = index.substr(0, pos + 3);
	}
	index = index.substr(index.find_last_of("/"));
	return "/cgi-bin" + index;
}

std::string CGI::_parsePathInfo(std::string index)
{
	int pos_begin = index.find(".py/");
	if (pos_begin == -1)
		return std::string();
	int pos_end = index.find("?");
	if (pos_end == -1)
		index = index.substr(pos_begin + 3);
	else
	{
		index = index.substr(0, pos_end);
		index = index.substr(pos_begin + 3);
	}
	return index;
}

std::string CGI::_parseContentType()
{
	if (_env.find("HTTP_CONTENT_TYPE") == _env.end())
		return std::string();
	return _env["HTTP_CONTENT_TYPE"];
}
std::string CGI::_parseContentLength()
{
	if (_env.find("HTTP_CONTENT_LENGTH") == _env.end())
		return std::string();
	return _env["HTTP_CONTENT_LENGTH"];
}

std::string CGI::_parsePathTranslated()
{
	std::string absolute_path = getcwd(NULL, 0);
	return absolute_path + "/www" + _env["SCRIPT_NAME"];
}

std::string CGI::_parseRemoteHost()
{
	int pos = _env["HTTP_HOST"].find(":");
	std::string host = _env["HTTP_HOST"].substr(0, pos);
	int count = std::count(host.begin(), host.end(), '.');
	if (count == 3)
		return std::string();
	else
		return host;
}