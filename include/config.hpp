
#include <string>
#include <vector>

#ifndef PLNR_CONFIG_H
#define PLNR_CONFIG_H

namespace plnr
{

    struct component {
        std::string                             name;
        int                                     size;
        std::vector<std::string>                configurations;
    };


    class config {
    
    private:
        struct _component {
            std::string                         name,
                                                src;
            int                                 size;
            std::vector<std::string>            fixed_arguments;
            std::vector<int>                    range_arguments;    
            std::vector<std::pair<int, int> >   positions;};

        std::vector<struct _component>          _settings;
        std::vector<struct component>           settings;


        std::string                             file,
                                                directory;
        int                                     frequency;
        double                                  h;
        bool                                    loaded,
                                                configured;

        void read_format_line(std::ifstream& file, std::string &line, int &line_number);
        bool trim_compare(const std::string& _left, const std::string& _right);
        bool file_exists(const std::string& name);
        void nested_combinations(struct _component _component, std::string result_nested, std::vector<std::string>& combinations, int i, int shift, int last);
        void nested_configurations(std::vector<std::vector<std::string> > __configurations, std::vector<std::vector<std::string> >& _configurations, std::vector<std::string> result_nested, int i, int l);

    public:
        config(const std::string& _file);

        config(component __component, double _frequency, double _h);

        config(double _frequency, double _h);

        ~config();

        double get_frequency();

        double get_h();

        void load();

        void configure();

        std::vector<struct component> components();

        void add_component(component __component);

        template<typename... params>
        void add_configuration(component __component, params... _params);
    };
}

#endif
