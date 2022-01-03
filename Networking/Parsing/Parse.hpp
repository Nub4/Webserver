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
            std::vector<std::string>    method;
            std::string                 name;
            std::string                 root;
            std::string                 upload_enable;
            std::string                 upload_path;
            std::string                 cgi_extension;
            std::string                 cgi_path;
            std::string                 client_max_body_size;
        };

        struct serverBlock
        {
            std::string                 listen;
            std::string                 server_name;
            std::map<int, std::string>  error_page;
            std::string                 autoindex;
            std::vector<std::string>    method;
            std::vector<locationBlock>  location;
            std::string                 root;
            std::string                 client_max_body_size;
        };

    public:
        Parse();

        void                        readFile(char *conf, std::string path);
        void                        readBinaryFile(char *conf, std::string path);
        void                        printStructs();
        void                        getConfigurationData();
        std::vector<serverBlock>    getServerContent();

    protected:
        std::vector<serverBlock>    _serverContent;
        std::string                 _conf_file;
        std::vector<unsigned char>  _binary_file;
        std::vector<std::string>    _server_names;
        std::vector<std::string>    _location_names;
        
        void                        _get_location(int start, int end, std::string temp, struct locationBlock *loct);
        bool                        _is_validName(std::string name);
        bool                        _is_validLocationName(std::string name);
        int                         _ft_isprint(int c);
        int                         _checkClosingBracket(int pos, std::string s);
        int                         _parseServer(int start_pos);
        void                        _msg_exit(std::string s);
        void                        _get_conf(int start, int end);
        int                         _ft_isdigit(int c);
        int                         _isNumber(std::string str);
        int                         _parseLocation(int start_pos, std::string temp, struct locationBlock *loct);
        void                        _checkServerValues();
        void                        _checkBackChar(std::string *x, std::string name);
        void                        _check_method(std::vector<std::string> *v);
        void                        _check_autoindex(std::string *x);
        void                        _check_error_page(std::map<int, std::string> *m);
        void                        _insert_error_page(std::vector<std::string> words, std::string x, struct serverBlock *serv);
};

#endif