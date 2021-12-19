#ifndef LISTEN_HPP
# define LISTEN_HPP

# include "Bind.hpp"

namespace ft
{
    class Listen : public Bind
    {
        public:
            Listen(int domain, int service, int protocol, int port, u_long interface, int backlog);
            ~Listen();

            void    startListening();

        private:
            int _backlog;
            int _listening;
    };
}

#endif