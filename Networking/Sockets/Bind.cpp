#include "Bind.hpp"

ft::Bind::Bind(int domain, int service, int protocol, int port, u_long interface)
: Socket(domain, service, protocol, port, interface)
{
    // Connect to the network and confirm success
    // Pass parameters to parent constructor for instantiation
    connectToNetwork(getSock(), getAddress());
    testConnection(_binding);
}

ft::Bind::~Bind() {}

void     ft::Bind::connectToNetwork(int sock, struct sockaddr_in address){
    _binding = bind(sock, (struct sockaddr *)&address, sizeof(address)); }

int     ft::Bind::getBind() { return _binding; }