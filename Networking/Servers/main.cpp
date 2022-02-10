#include "Server.hpp"
#include "CGI.hpp"

void    exit_and_error(std::string str)
{
	std::cout << str << std::endl;
	exit(1);
}

int     main(int ac, char **av)
{
  if (ac != 2)
		exit_and_error("Error: configuration filename for the argument");

    Parse parse;
    parse.readConfFile(av[1]);
    parse.getConfigurationData();

    Server server(parse.getServerContent());

    return 0;
}