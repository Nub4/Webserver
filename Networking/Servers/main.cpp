#include "Server.hpp"
#include "../Parsing/Parse.hpp"

int     main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Wrong number of arguments\n";
        return 1;
    }

    Server server;
    Parse parse;

    parse.setFile(av[1]);
    parse.setMap();

    server.setup_server();
    server.run_server();
    return 0;
}