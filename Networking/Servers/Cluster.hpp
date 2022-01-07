#pragma once

#include "Server.hpp"
#include "../Parsing/Parse.hpp"
#include "../Utils/Utils.hpp"

class Cluster
{
	public:
		Cluster(std::vector<Parse::serverBlock> servers);
		~Cluster();

		void setup_cluster();
		void run_cluster();
	
	private:
		std::vector<Server> 	_servers;
		std::vector<int>		_serverSockets;
};