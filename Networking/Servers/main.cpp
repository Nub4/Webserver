#include "Server.hpp"

void    exit_and_error(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

int     main(int ac, char **av, char **env)
{
    if (ac != 2)
        exit_and_error("Error: configuration filename for the argument");

    std::string path;
    for (int i = 0; env[i] != NULL; i++)
        if (strncmp(env[i], "PWD", 3) == 0)
            path = env[i];
    path.erase(0, 4);

    Server server;
    Parse parse;

    parse.readFile(av[1], path);
    parse.getConfigurationData();
    parse.printStructs();

    // server.setup_server();
    // server.run_server();
    return 0;
}