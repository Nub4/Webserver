#include "RunClient.hpp"

int     main(int ac, char **av)
{
    if (ac != 2)
        return 0;
    // Create socket file descriptor
    Socket s(AF_INET, SOCK_STREAM, 0);
    // Create address structure
    Address a("client");
    // Connect to network
    Connect c(s.getSocket(), a.getServAddr());
    // Run client
    RunClient rc(s.getSocket(), av[1]);
    return 0;
}