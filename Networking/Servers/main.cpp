#include "Server.hpp"
#include "../Parsing/Parse.hpp"

void    exit_and_error(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

int     main(int ac, char **av)
{
    if (ac != 2)
        exit_and_error("Error: configuration filename for the argument");

    Server server;
    Parse parse;

    parse.setFile(av[1]);
    parse.setConfs();

    server.setup_server();
    server.run_server();
    return 0;
}