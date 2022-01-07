#include "Cluster.hpp"

Cluster::Cluster(std::vector<Parse::serverBlock> servers)
{
	// Construct the amount of servers needed inside a vector
	for (std::vector<Parse::serverBlock>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		Server server(*it);
		_servers.push_back(server);
		// Assign server sockets from servers to vector of server sockets here in cluster
		_serverSockets.push_back(server.getServerSocket());
		server.run_server();
	}
}

Cluster::~Cluster() {}

void Cluster::run_cluster()
{
}

// //integrate this function
// void    Cluster::run_cluster()
// {
//     fd_set readfds;

//     FD_ZERO(&readfds);
//     FD_SET(_serverSocket, &readfds);
//     _fdmax = _serverSocket;

//     while (1)
//     {
//         fd_set copy = readfds;
//         _check(select(_fdmax + 1, &copy, NULL, NULL, NULL), "select");

//         for (int i = 0; i <= _fdmax; i++)
//         {
//             if (FD_ISSET(i, &copy))
//             {
//                 if (i == _serverSocket)
//                 {
//                     int clientSocket = _accept();
//                     FD_SET(clientSocket, &readfds);
//                     if (clientSocket > _fdmax)
//                         _fdmax = clientSocket;
//                 }
//                 else
//                 {
//                     _handler(i);
//                     FD_CLR(i, &readfds);
//                 }
//             }
//         }
//     }
// }