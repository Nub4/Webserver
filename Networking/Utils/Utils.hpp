#ifndef UTILS_HPP
# define UTILS_HPP

# include "../../includes/libc.hpp"

class Utils
{
    public:
        Utils();

        std::vector<unsigned char>  readBinaryFile(char *conf, std::string path);

    protected:
        int                         _errorCode;
        std::string                 _root;
        std::string                 _index;
        int                         _max_size;
        std::vector<std::string>    _method;
        std::map<int, std::string>  _error_page;

        bool    _isCorrectHost(std::string host);
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

        int             _sendall(int clientSocket, const char *buf, int *size);
        void            _setErrorPages();
        std::string     _getErrorPage(std::string *type);
        std::string     _getFile(std::ifstream *f);
        std::string     _getDefaultFile(std::string *type);
        std::string     _getCacheControl();
        std::string     _getContentType(std::string type);
        std::string     _getContentLength(int size);
        std::string     _getStatus(int err_code);
};

#endif