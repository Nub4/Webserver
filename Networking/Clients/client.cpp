#include "RunClient.hpp"

int     main()
{
    // Create socket file descriptor
    Socket s(AF_INET, SOCK_STREAM, 0);
    // Create address structure
    Address a("client");
    // Connect to network
    Connect c(s.getSocket(), a.getServAddr());
    // Run client
    RunClient rc(s.getSocket());
    return 0;
}