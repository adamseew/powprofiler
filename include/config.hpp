
#include <string>
#include <vector>

#ifndef PLNR_CONFIG_H
#define PLNR_CONFIG_H

namespace plnr
{
    class config {
    
    private:
        struct component {
        std::string                     name,
                                        src;
        std::vector<int>                range_arguments;
        std::vector<std::string>        fixed_arguments;    };

        std::vector<struct component>   settings;

        std::string                     file,
                                        directory;
        int                             frequency;
        double                          h;

        void read_format_line(std::ifstream& file, std::string &line, int &line_number);
        bool trim_compare(const std::string& _left, const std::string& _right);
        bool file_exists(const std::string& name);

    public:

        config(const std::string& _file);

        ~config();

        void load();
    };
}

#endif
