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

	char *c_env[_envMap.size() + 1];
	for (size_t i = 0; i < _envMap.size(); i++)
		c_env[i] = (char *)_envVect[i].c_str();
	c_env[_envMap.size()] = NULL;
///////////////////////
	// for (int j = 0; c_env[j] != NULL; j++)
	// 	std::cout << c_env[j] << "\n";
///////////////////////

	char *c_args[_args.size() + 1];
	for (size_t i = 0; i < _args.size(); i++)
		c_args[i] = (char *)_args[i].c_str();
	c_args[_args.size()] = NULL;

///////////////////////
	for (int j = 0; c_args[j] != NULL; j++)
		std::cout << c_args[j] << "\n";
///////////////////////

	int fd_file = open("temp.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
	if (fd_file == -1)
		return 1;
	pid_t pid = fork();
	if (pid == -1)
		return 1;
	else if (pid == 0)
	{
		dup2(fd_file, STDOUT_FILENO);
		close(fd_file);
		char const *pathname = _envMap["PATH_TRANSLATED"].c_str();
		if (execve(pathname, c_args, c_env) == -1)
		{
			std::cerr << RED << "execve\n" << RESET;
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
	_envMap["SERVER_PROTOCOL"] = parsed[2];
	_envMap["SERVER_NAME"] = server.listen[1];
	_envMap["SERVER_PORT"] = server.listen[0];
	_envMap["REQUEST_METHOD"] = parsed[0];
	_envMap["SCRIPT_NAME"] = _parseScriptName(index);
	_envMap["PATH_INFO"] = _parsePathInfo(index);
	_envMap["PATH_TRANSLATED"] = _parsePathTranslated();
	_envMap["QUERY_STRING"] = _parseQueryString(parsed);
	_envMap["REMOTE_HOST"] = _parseRemoteHost(); 
	_envMap["REMOTE_ADDR"] = std::string();
	_envMap["AUTH_TYPE"] = std::string();
	_envMap["REMOTE_USER"] = std::string();
	_envMap["CONTENT_TYPE"] = _parseContentType();
	_envMap["CONTENT_LENGTH"] = _parseContentLength();
	_envMap["SERVER_SOFTWARE"] = "WEBSERV/42.19";
	_envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	_parseEnvMapToVect();
	_parseQueryStringToArgs();

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
		_envMap[var] = it->second;
	}
}

void CGI::_parseEnvMapToVect()
{
	std::map<std::string, std::string>::iterator it = _envMap.begin();
	for (; it != _envMap.end(); it++)
	{
		std::string tmp = it->first + '=' + it->second;
		_envVect.push_back(tmp);
	}
}

std::string CGI::_parseQueryString(std::vector<std::string> parsed)
{
	std::string queryString;
	if (parsed[0] == "GET")
	{
		queryString = parsed[1];
		int pos = queryString.find("?");
		if (pos == -1)
			return std::string();
		return queryString.substr(pos + 1);
	}
	else if (parsed[0] == "POST")
	{
		queryString = parsed[parsed.size() - 1];
	}
	return queryString;
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
	pos = index.find_last_of("/");
	if (pos != -1)
	{
		index = index.substr(index.find_last_of("/"));
		return "/cgi-bin" + index;
	}
	return "/cgi-bin/" + index;
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
	if (_envMap.find("HTTP_CONTENT_TYPE") == _envMap.end())
		return std::string();
	return _envMap["HTTP_CONTENT_TYPE"];
}
std::string CGI::_parseContentLength()
{
	if (_envMap.find("HTTP_CONTENT_LENGTH") == _envMap.end())
		return std::string();
	return _envMap["HTTP_CONTENT_LENGTH"];
}

std::string CGI::_parsePathTranslated()
{
	std::string absolute_path = getcwd(NULL, 0);
	return absolute_path + "/www" + _envMap["SCRIPT_NAME"];
}

std::string CGI::_parseRemoteHost()
{
	std::string host;
	int pos = _envMap["HTTP_HOST"].find(":");
	if (pos != -1)
		host = _envMap["HTTP_HOST"].substr(0, pos);
	int count = std::count(host.begin(), host.end(), '.');
	if (count == 3)
		return std::string();
	else
		return host;
}

void CGI::_parseQueryStringToArgs()
{
	_args.push_back(_envMap["PATH_TRANSLATED"]);
	if (_envMap["QUERY_STRING"] == std::string())
		return;
	std::string queryString = _envMap["QUERY_STRING"];
	for (int pos = queryString.find("&"); pos != -1; pos = queryString.find("&"))
	{
		std::string tmp_arg = queryString.substr(0, pos);
		tmp_arg.erase(0, tmp_arg.find("=")+1);
		_args.push_back(tmp_arg);
		queryString.erase(0, pos+1);
	}
	queryString.erase(0, queryString.find("=")+1);
	_args.push_back(queryString);
////////////////////
	// for (std::vector<std::string>::iterator it = _args.begin(); it != _args.end(); it++)
	// 	std::cout << *it << "\n";
}