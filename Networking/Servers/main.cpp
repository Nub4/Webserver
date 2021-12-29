#include "Server.hpp"

int     main()
{
    Server server;

    server.setup_server();
    server.run_server();
    return 0;
}