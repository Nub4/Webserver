#ifndef UTILS_HPP
# define UTILS_HPP

# include "../../includes/libc.hpp"

# define UNESCAPED_CHARS "-_.!~*'()"

class Utils
{
    public:
        Utils();

    protected:
        std::string                 _location;
        int                         _errorCode;
        std::string                 _root;
        std::string                 _index;
        int                         _max_size;
        std::vector<std::string>    _method;
        std::map<int, std::string>  _error_page;
        std::string                 _autoindex;
        std::map<int, std::string>  _redirect;
        std::string                 _upload;

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
        void    _check_redirect(std::vector<std::string> *v);

        int             _sendall(int clientSocket, const char *buf, int *size);
        std::string     _getErrorPage(std::string *type);
        std::string     _getFile(std::ifstream *f);
        std::string     _getDefaultFile(std::string *type);
        std::string     _getCacheControl();
        std::string     _getContentType(std::string type);
        std::string     _getContentLength(int size);
        std::string     _getStatus(int err_code);
        std::string     _getLocation(std::string url);
        std::string     _getAutoindexHtml(std::string path, std::string uri, std::string *type);
        std::string     _getFileString(std::string path);
        // bool            _shouldBeEscaped(char c);
        // std::string     _getEscapedChar(char c);
        // std::string     _encodeURIComponent(std::string src);
        std::string     _decodeURIComponent(std::string encodedURI);
};

#endif