#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <iostream>

class Socket
{
    public:
        Socket(int domain, int service, int protocol, int port, u_long interface);

        struct sockaddr_in  getAddress();
        int                 getSock();
        virtual void        connectToNetwork(int sock, struct sockaddr_in address) = 0;
        void                testConnection(int item);

    private:
        int                 _sock;
        struct sockaddr_in  _address;
};

#endif