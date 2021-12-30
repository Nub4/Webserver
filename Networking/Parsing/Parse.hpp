#ifndef PARSE_HPP
# define PARSE_HPP

# include "../../includes/libc.hpp"

class Parse
{
    public:
        Parse();

        void    setFile(char *conf);
        size_t      lineCount(std::string str);
        bool        is_validName(std::string name);
        void    setMap();

    private:
        std::string                                         _conf_file;
        void                                                _error_manage(std::string str);
        std::vector<std::string>                            _valid_names;
        std::map<std::string, std::vector<std::string> >    _conf_map;
};

#endif