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
            int                 getConnection();

            void                setConnection(int item);

            virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
            void        testConnection(int item);

        private:
            int                 _connection;
            int                 _sock;
            struct sockaddr_in  _address;
    };
}

#endif