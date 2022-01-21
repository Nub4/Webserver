#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sys/wait.h>
#include "../Parsing/Parse.hpp"

class CGI
{
    public:
        CGI();
        CGI(Parse::serverBlock server, std::vector<std::string> parsed, std::string index);
        ~CGI();

		void runCGI();

    private:
        std::map<std::string, std::string>  _env;				
        std::string                         _getUserAgent(std::vector<std::string> parsed);
};