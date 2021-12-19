#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

namespace ft
{
    class Socket
    {
        public:
            Socket(int domain, int service, int protocol, int port, u_long interface);
            ~Socket();

            struct sockaddr_in  getAddress();
            int                 getSock();

            // This function connects to the network using either bind() or connect() from sys/socket.h
            virtual void        connectToNetwork(int sock, struct sockaddr_in address) = 0;
            void                testConnection(int item);

        private:
            // Once instantiad, sock contains the address od a socket object on the network.
            // this should be a non-negative number.
            int                 _sock;
            // The address variable is a struct that contains information about the protocol, type, and interface
            // of a given socket. These parameters are specified and set in the constructor.
            struct sockaddr_in  _address;
    };
}

#endif