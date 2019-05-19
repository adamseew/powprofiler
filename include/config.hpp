
#include <string>
#include <vector>

#ifndef PLNR_CONFIG_H
#define PLNR_CONFIG_H

namespace plnr
{
    class config {
    
    private:
        struct component {
        std::string                             name,
                                                src;
        std::vector<std::string>                fixed_arguments;
        std::vector<int>                        range_arguments;    
        std::vector<std::pair<int, int> >       positions;      };

        std::vector<struct component>           settings;
        std::vector<std::vector<std::string> >  configurations;

        std::string                             file,
                                                directory;
        int                                     frequency;
        double                                  h;

        void read_format_line(std::ifstream& file, std::string &line, int &line_number);
        bool trim_compare(const std::string& _left, const std::string& _right);
        bool file_exists(const std::string& name);
        void nested_combinations(struct component _component, std::string result_nested, std::vector<std::string>& combinations, int i, int shift, int last);
        std::string nested_configurations(std::vector<std::vector<std::string> > __configurations, std::vector<std::vector<std::string> >& _configurations, int l);

    public:

        config(const std::string& _file);

        ~config();

        void load();

        void configure();
    };
}

#endif
