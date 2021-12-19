#include "Listen.hpp"

int     main()
{
    std::cout << "Starting...\n";
    std::cout << "Binding Socket...\n";
    ft::Bind bind = ft::Bind(AF_INET, SOCK_STREAM, 0, 81, INADDR_ANY);
    std::cout << "Listening Socket...\n";
    ft::Listen listen = ft::Listen(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    std::cout << "Success!\n";
}