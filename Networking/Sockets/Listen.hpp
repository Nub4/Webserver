#ifndef LISTEN_HPP
# define LISTEN_HPP

# include "Bind.hpp"

class Listen : public Bind
{
    public:
        Listen(int domain, int service, int protocol, int port, u_long interface, int backlog);
        virtual ~Listen();

        void    startListening();
        int     getListening();
        int     getBacklog();

    private:
        int _backlog;
        int _listening;
};

#endif