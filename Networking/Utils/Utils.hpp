#ifndef UTILS_HPP
# define UTILS_HPP

# include "../../includes/libc.hpp"

class Utils
{
    public:
        Utils();

        std::vector<unsigned char>  readBinaryFile(char *conf, std::string path);

    protected:
        void    _check_body_size(std::string *x);
        void    _check_listen(std::vector<std::string> *v);   
        int     _ft_isprint(int c);
        int     _checkClosingBracket(int pos, std::string s);
        void    _msg_exit(std::string s);
        int     _ft_isdigit(int c);
        int     _isNumber(std::string str);
        void    _checkBackChar(std::string *x, std::string name);
        void    _check_method(std::vector<std::string> *v);
        void    _check_autoindex(std::string *x);
        void    _check_error_page(std::map<int, std::string> *m);
};

#endif