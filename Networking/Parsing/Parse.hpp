#ifndef PARSE_HPP
# define PARSE_HPP

# include "../../includes/libc.hpp"

class Parse
{
    public:
        struct locationBlock
        {
            std::vector<std::string>    index;
            std::string                 autoindex;
            std::vector<std::string>    methods;
        };

        struct serverBlock
        {
            std::string                 listen;
            std::string                 server_name;
            std::map<int, std::string>  error_page;
            std::string                 autoindex;
            std::vector<std::string>    methods;
            std::vector<locationBlock>  location;
        };

    public:
        Parse();

        void    setFile(char *conf);
        void    setBinaryFile(char *conf);
        int     ft_isprint(int c);
        int     checkClosingBracket(int pos);
        int     parseServer(int start_pos);
        void    msg_exit(std::string s);
        void    get_conf(int start, int end);
        int     ft_isdigit(int c);
        int     isNumber(std::string str);
        void    printStructs();

    protected:
        std::vector<serverBlock>    _serverContent;
        std::string                 _conf_file;
        std::vector<unsigned char>  _binary_file;
        std::vector<std::string>    _server_names;
        std::vector<std::string>    _location_names;
        bool                        _is_validName(std::string name);
};

#endif