#include "Server.hpp"
#include "CGI.hpp"

class Test : public Utils
{
public:

    std::string _encodeURI(std::string src)
    {
        return _encodeURIComponent(src);
    }
    std::string getEscapedChar(char c)
    {
        return _getEscapedChar(c);
    }


    std::string getContent(std::string path, std::string uri)
    {
        std::string type;
        return _getAutoindexHtml(path, uri, &type);
    }
};

void    exit_and_error(std::string str)
{
	std::cout << str << std::endl;
	exit(1);
}

int     main(int ac, char **av)
{
    // if (ac != 2)
    //     exit_and_error("Error: configuration filename for the argument");

    // Parse parse;
    // parse.readConfFile(av[1]);
    // parse.getConfigurationData();

    // Server server(parse.getServerContent());
    (void)ac;
    (void)av;
    // DIR *dirp = opendir("/");
    // if (!dirp)
    // {
    //     std::cout << "problemos";
    //     std::cout << strerror(errno);
    //     return 0;
    // }
    // struct dirent *direntp = readdir(dirp);
    // while (direntp)
    // {
    //     std::cout << direntp->d_name << '\n';
    //     direntp = readdir(dirp);
    // }
    Test test;

    if (ac < 2)
        return 0;
    // std::cout << test.getEscapedChar(av[1][0]);
    std::cout << test._encodeURI(av[1]);
    return 0;
}
