#ifndef PARSE_HPP
# define PARSE_HPP

# include "../../includes/libc.hpp"

class Parse
{
    public:
        Parse();

        void    setFile(char *conf);
        void    setBinaryFile(char *conf);
        void    setConfs();

    protected:
        std::vector<std::map<std::string, std::vector<std::string> > >  _serverBlocks;
        std::string                                                     _conf_file;
        std::vector<unsigned char>                                      _binary_file;
        void                                                            _error_manage(std::string str);
        std::vector<std::string>                                        _valid_names;
        std::map<std::string, std::vector<std::string> >                _serverContent;
        size_t                                                          _serverLineCount(std::string str, std::string c, std::string z);
        bool                                                            _is_validName(std::string name);
        int                                                             _countWord(std::string s, std::string word);
};

#endif