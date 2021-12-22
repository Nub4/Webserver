#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include "../../includes/libc.hpp"
# include "Socket.hpp"

class Address
{
    public:
        Address();

        struct sockaddr_in  getAddress();

    private:
        struct sockaddr_in  _address;
};

#endif