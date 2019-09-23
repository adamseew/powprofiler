
#include "utility.hpp"

#include <string>
#include <vector>

#ifndef PLNR_CONFIG_H
#define PLNR_CONFIG_H

#define __D_DEFAULT_FREQUENCY__     10
#define __D_DEFAULT_H__             0.01

namespace plnr
{

    struct component {
        component(void) : 
            name(""), src(""), size(0), runtime(0) { }
        component(const std::string& _name) : 
            name(_name), src(""), size(0), runtime(0) { }
        component(const std::string& _name, int _runtime) : 
            name(_name), src(""), size(0), runtime(_runtime) { }

        std::string                             name,
                                                src;
        int                                     size,
                                                runtime;
        std::vector<std::string>                configurations;
    };


    class config {
    
    private:
        struct _component {
            std::string                         name,
                                                src;
            int                                 size,
                                                runtime;
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
        void nested_combinations(struct _component _component, std::string result_nested, std::vector<std::string>& combinations, int i, int shift, int last);
        void nested_configurations(std::vector<std::vector<std::string> > __configurations, std::vector<std::vector<std::string> >& _configurations, std::vector<std::string> result_nested, int i, int l);

    public:
        config(const std::string& _file);
        
        config();
        
        config(double _frequency);
        
        config(double _frequency, double _h);
        
        config(double _frequency, const std::string& _directory);
        
        config(double _frequency, double _h, const std::string& _directory);

        ~config();

        double get_frequency();

        double get_h();

        const std::string get_directory();

        void load();

        void configure();

        std::vector<struct component> components();

        std::vector<size_t> configurations(const component& __component);

        void add_component(component __component);

        struct component get_component(std::string name);

        std::string get_configuration(const component& __component, size_t _configuration);

        int get_size(std::string name);

        // returns configuration id

        size_t add_configuration(const component &__component, const std::vector<std::string> &_configurations);

        // variadic overload of the above function, for comodity (so it can be invoked with a variable number of parameters)

        template<typename... params>
        size_t add_configuration(const component &__component, const std::string &_param, const params&... _params)  {

            std::vector<std::string>    variadic_vector;

            int                         dummy[] = { 0, (variadic_vector.push_back(_params), 0)... };
            (void) dummy;
            variadic_vector.insert(variadic_vector.begin(), _param);

            return add_configuration(__component, variadic_vector);
        }

        template<typename... params>
        size_t add_configuration(const component &__component, int _param, const params&... _params)  {

            std::vector<std::string>    variadic_vector;

            int                         dummy[] = { 0, (variadic_vector.push_back(std::to_string(_params)), 0)... };
            (void) dummy;
            variadic_vector.insert(variadic_vector.begin(), std::to_string(_param));

            return add_configuration(__component, variadic_vector);
        }

        template<typename... params>
        size_t add_configuration(const component &__component, double _param, const params&... _params)  {

            std::vector<std::string>    variadic_vector;

            int                         dummy[] = { 0, (variadic_vector.push_back(std::to_string(_params)), 0)... };
            (void) dummy;
            variadic_vector.insert(variadic_vector.begin(), std::to_string(_param));

            return add_configuration(__component, variadic_vector);
        }
    };
}

#endif
