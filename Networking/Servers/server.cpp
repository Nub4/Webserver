#include "RunServer.hpp"

int     main()
{
    // Create socket file descriptor
    Socket s(AF_INET, SOCK_STREAM, 0);
    // Create address structure
    Address a;
    // Bind file descriptor
    Bind b(s.getSocket(), a.getAddress());
    // Listen file descriptor
    Listen l(s.getSocket());
    // Run server
    RunServer rs(s.getSocket(), a.getAddress());
    return 0;
}