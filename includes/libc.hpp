#ifndef LIBC_HPP
# define LIBC_HPP

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
# include <sys/select.h>
# include <sys/time.h>
# include <fcntl.h>
# include <map>
# include <cstring>
# include <list>
# include <iterator>
# include <algorithm>
# include <cerrno>
# include <dirent.h>
# include <cstdio>

# define PORT 3490
# define BACKLOG 1024
# define BUFF_SIZE 8000000

# define YELLOW "\033[33m"
# define GREEN	"\033[32m"
# define RED	"\033[31m"
# define RESET	"\033[0m"

#endif