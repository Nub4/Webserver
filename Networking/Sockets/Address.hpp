#ifndef ADDRESS_HPP
# define ADDRESS_HPP

# include "../../includes/libc.hpp"
# include "Socket.hpp"

class Address
{
    public:
        Address(std::string str);

        struct sockaddr_in  getAddress();
        struct sockaddr_in  getServAddr();

    private:
        struct sockaddr_in  _address;
        struct sockaddr_in  _serv_addr;

};

#endif