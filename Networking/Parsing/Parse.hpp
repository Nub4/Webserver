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
            std::string                 name;
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

        void    readFile(char *conf);
        void    readBinaryFile(char *conf);
        void    printStructs();
        void    getConfigurationData();

    protected:
        std::vector<locationBlock>  _locationContent;
        std::vector<serverBlock>    _serverContent;
        std::string                 _conf_file;
        std::vector<unsigned char>  _binary_file;
        std::vector<std::string>    _server_names;
        std::vector<std::string>    _location_names;
        bool                        _is_validName(std::string name);
        bool                        _is_validLocationName(std::string name);
        int                         _ft_isprint(int c);
        int                         _checkClosingBracket(int pos);
        int                         _parseServer(int start_pos);
        void                        _msg_exit(std::string s);
        void                        _get_conf(int start, int end);
        int                         _ft_isdigit(int c);
        int                         _isNumber(std::string str);
        void                        _erase_separator();

};

#endif