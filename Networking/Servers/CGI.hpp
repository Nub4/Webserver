#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/wait.h>
#include <algorithm>
#include "../Parsing/Parse.hpp"

class CGI : public Utils
{
    public:
        CGI();
        CGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index);
        ~CGI();

		int runCGI();

    private:
		std::map<std::string, std::string>  _env;				
        
		void _initEnvCGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index);
		std::map<std::string, std::string>	_parseClientVariables(std::vector<std::string> parsed);
		void _parseClientToEnvVariables(std::map<std::string, std::string> client_header);
		std::string _parseQueryString(std::string url);
		std::string _parseScriptName(std::string index);
		std::string	_parsePathInfo(std::string index);
		std::string _parseContentType();
		std::string _parseContentLength();
		std::string _parsePathTranslated();
		std::string _parseRemoteHost();
		std::string _parseRemoteAddr(); // where do we get this guy?
};