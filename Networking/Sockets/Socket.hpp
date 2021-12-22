#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <iostream>
# include <string>
# include <unistd.h>
# include <istream>
# include <sstream>
# include <fstream>
# include <vector>
# include <streambuf>
# include <iterator>
# include <arpa/inet.h>

class Socket
{
    public:
        Socket();
        Socket(int domain, int type, int protocol);

        int     getSocket();
        void    testConnection(int item, std::string str);

    private:
        int                 _socket;
};

#endif