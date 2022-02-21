#ifndef PARSE_HPP
# define PARSE_HPP

# include "../Utils/Utils.hpp"

class Parse : public Utils
{
    public:
        struct locationBlock
        {
            std::vector<std::string>    index;
            std::string                 autoindex;
            std::vector<std::string>    method;
            std::string                 name;
            std::string                 root;
            std::vector<std::string>    redirect;
            std::string                 upload;
        };

        struct serverBlock
        {
            std::vector<std::string>    listen;
            std::string                 server_name;
            std::map<int, std::string>  error_page;
            std::vector<locationBlock>  location;
            std::string                 client_max_body_size;
        };

    public:
        Parse();

        void                        readConfFile(char *conf);
        void                        printStructs();
        void                        getConfigurationData();
        std::vector<serverBlock>    getServerContent();

    protected:
        std::vector<serverBlock>    _serverContent;
        std::string                 _conf_file;
        std::vector<std::string>    _server_names;
        std::vector<std::string>    _location_names;

        void    _check_same_host();  
        void    _get_location(int start, int end, std::string temp, struct locationBlock *loct);
        bool    _is_validName(std::string name);
        bool    _is_validLocationName(std::string name);
        int     _parseServer(int start_pos);
        void    _get_conf(int start, int end);
        int     _parseLocation(int start_pos, std::string temp, struct locationBlock *loct);
        void    _checkServerValues();
        void    _insert_error_page(std::vector<std::string> words, std::string x, struct serverBlock *serv);
};

#endif