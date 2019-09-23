
#include "../include/utility.hpp"
#include "../include/config.hpp"

#include <functional>
#include <algorithm>
#include <cstdarg>
#include <fstream>
#include <cmath> 

using namespace plnr;

using std::invalid_argument;
using std::runtime_error;
using std::logic_error;
using std::to_string;
using std::exception;
using std::vector;
using std::string;
using std::hash;
using std::pair;

config::config(const string& _file) {

    /// calling powprof by passing a configuration file, this mean that user have to call load to load data from file, and configure later to generate all the configurations from file. Note: one can run the configuration file and the still decide to call powprof in the 'library' modality

    frequency = __D_DEFAULT_FREQUENCY__;
    h = __D_DEFAULT_H__;
    directory = "";
    file = _file;
    loaded = false;
    configured = false;
}

/// following constructors are used when powprof is compiled and used as a library, thus configurations run as they are added

config::config() {

    frequency = __D_DEFAULT_FREQUENCY__;
    h = __D_DEFAULT_H__;
    directory = "";

    // so we don't need to read from file, one will add the components and their configurations manually, in the 'library' modality

    loaded = true;
    configured = true;
}

config::config(double _frequency) {

    frequency = _frequency;
    h = __D_DEFAULT_H__;
    directory = "";

    // so we don't need to read from file, one will add the components and their configurations manually, in the 'library' modality

    loaded = true;
    configured = true;
}

config::config(double _frequency, double _h) {

    frequency = _frequency;
    h = _h;
    directory = "";
    loaded = true;
    configured = true;
}

config::config(double _frequency, const string& _directory) {

    frequency = _frequency;
    h = __D_DEFAULT_H__;
    directory = _directory;
    loaded = true;
    configured = true;
}


config::config(double _frequency, double _h, const string& _directory) {

    frequency = _frequency;
    h = _h;
    directory = _directory;
    loaded = true;
    configured = true;
}

config::~config() { 

    vector<struct _component>().swap(_settings);
    vector<struct component>().swap(settings);
}

double config::get_frequency() {
    return frequency;
}

double config::get_h() {
    return h;
}

const string config::get_directory() {
    return directory;
}

void config::load() {

    int                 i,
                        line_number =       0,
                        components_count =  0,
                        min,
                        max,
                        step;
    bool                was_previous_range;

    string              line;
    vector<string>      property_value;

    struct _component    __component;

    if (loaded)
        return;

    if (!utility_file_exists(file))
        throw logic_error("configuration file " + file + " does not exist");

    std::ifstream       input_cfg(file);

    read_format_line(input_cfg, line, line_number);

    // first line should be [settings]

    if (!trim_compare(line, "[settings]"))
        throw runtime_error("configuration file bad format line " + to_string(line_number) + ". File must start with [settings]");

    for ( ; ; ) {

        read_format_line(input_cfg, line, line_number);

        if (input_cfg.peek() == EOF)
            break;

        if (trim_compare(line, "[components]"))
            // line containing different components configuration ([components]) reached

            break;
        
        // expecting properties
        
        property_value = utility_split(line, '=');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

        property_value.at(1) = utility_trim(property_value.at(1));

        if (trim_compare(property_value.at(0), "frequency"))
            try {
                frequency = stoi(property_value.at(1));
                if (frequency <= 0)
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Frequency must be a positive integer");        
            } catch (exception &_exception) {
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected integer value but found " + line);    
            }

        else if (trim_compare(property_value.at(0), "h"))
            try {
                h = stof(property_value.at(1));
                if (frequency <= 0)
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". H must be a positive float");        
            } catch (exception &_exception) {
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected float value but found " + line);    
            }

        else if (trim_compare(property_value.at(0), "directory"))
            directory = property_value.at(1);

        else
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Property " + utility_trim(property_value.at(0)) + " not recognized");
        
    }

    read_format_line(input_cfg, line, line_number);

    if (line[0] != '[')
        throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected a component description but found " + line);

    for ( ; ; ) {
        // expecting new component

        property_value = utility_split(line, '.');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected [component.name] but found " + line);

        __component.name = utility_trim(property_value.at(1));
        __component.name = __component.name.substr(0, __component.name.size() - 1);

        read_format_line(input_cfg, line, line_number);

        // expecting component source

        property_value = utility_split(line, '=');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

        __component.src = utility_trim(property_value.at(1));

        if (!utility_file_exists(__component.src))
            throw logic_error("configuration file error line " + to_string(line_number) + ". Source file " + __component.src + " does not exist");

        __component.fixed_arguments.clear();
        __component.range_arguments.clear();
        __component.positions.clear();
        was_previous_range = false;

        __component.size = 1;

        // expecting a number of possible arguments. min,max,step for each argument         

        for (read_format_line(input_cfg, line, line_number);
             line[0] != '[' && line.size() > 0;
             read_format_line(input_cfg, line, line_number)
            ) {

            property_value = utility_split(line, '=');

            if (property_value.size() != 2)
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

            if (trim_compare(property_value.at(0), "range")) {
                
                if (was_previous_range)
                    __component.positions.at(__component.positions.size() - 1).first = __component.range_arguments.size() + 3;
                else
                    __component.positions.emplace_back(__component.range_arguments.size() + 3, __component.range_arguments.size());

                was_previous_range = true;

                property_value = utility_split(property_value.at(1), ',');

                if (property_value.size() != 3)
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value,value,value but found " + line);

                try {
                    min = stoi(utility_trim(property_value.at(0)));
                    max = stoi(utility_trim(property_value.at(1)));
                    __component.range_arguments.push_back(min);
                    __component.range_arguments.push_back(max);
                } catch (exception &_exception) {
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected integer value but found " + line);    
                }

                try {
                    property_value.at(2) = utility_trim(property_value.at(2));

                    if (property_value.at(2)[0] == 'p') {
                        property_value = utility_split(property_value.at(2), '(');

                        // if the line start by p, it means that a pow operator is involed. To save this operator into the data struct, the integer is stored as negative (just for convenience!)
                        
                        step = (-1) * stoi(property_value.at(1).substr(0, property_value.at(1).size()));
                    } else
                        step = stoi(property_value.at(2));
                    
                    __component.range_arguments.push_back(step);

                } catch (exception &_exception) {
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value or pow(value) with integer value but found " + line);
                }

                __component.size++;

                // very basic check if the range is valid

                if ( max < 0 || 
                     min < 0 ||
                    (max - min) < 0 || 
                    (step > 0 && (max - min) < step) || 
                    (step < 0 && (max - min) < step * step
                   )) {
                    throw logic_error("configuration file error line " + to_string(line_number) + ". Invalid format for range argument. Use min,max,step pattern where min,max are positive integers, step can be a natural or a power expressed as pow(step)");
                }

            } else if (trim_compare(property_value.at(0), "fixed")) {
                
                __component.positions.emplace_back(0, __component.fixed_arguments.size());
                was_previous_range = false;

                __component.fixed_arguments.push_back(utility_trim(property_value.at(1)));

            } else if  (trim_compare(property_value.at(0), "runtime")) {
                
                try {
                    __component.runtime = stoi(utility_trim(property_value.at(1)));
                } catch (exception &_exception) {
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected integer value but found " + line);    
                }

                if (__component.runtime <= 0)
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Cannot run for " + to_string(__component.runtime) + " ms");  

            } else {

                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Property " + utility_trim(property_value.at(0)) + " not recognized");
            }

            if (input_cfg.peek() == EOF)
                break;
        }

        _settings.push_back(__component);
        ++components_count;

        if (input_cfg.peek() == EOF)
            break;
    }
    
    // this is allowed, as soon as add_component (and/or add_configuration) is invoked

    // if (components_count < 1)
    //    throw logic_error("configuration file error line " + to_string(line_number) + ". At least one component expected");

    input_cfg.close();
    loaded = true;
}

void config::configure() {

    int                 i;

    struct component    _component;

    vector<string>      arguments_combinations,
                        _configurations;

    if (configured)
        return;

    for (auto &__component : _settings) {
        _component.configurations.clear();
        _component.src = __component.src;
        _component.configurations.push_back("");

        for (auto position : __component.positions) {

            if (position.first == 0) {

                for (i = 0; i < _component.configurations.size(); i++)
                    _component.configurations.at(i) += " " + __component.fixed_arguments.at(position.second);

            } else {

                arguments_combinations.clear();
                nested_combinations(__component, "", arguments_combinations, __component.range_arguments.at(position.second), position.second, position.first); 
                _configurations.clear();

                for (i = 0; i < _component.configurations.size(); i++) {
                    for (auto combination : arguments_combinations)
                        _configurations.push_back(_component.configurations.at(i) + combination.erase(0, 1)); // erase is an easy fix to delete the initial space
                }

                if (_configurations.size() > _component.configurations.size()) {
                    _component.configurations.clear();
                    _component.configurations.insert(_component.configurations.end(), _configurations.begin(), _configurations.end());
                }
            }
        }

        _component.name = __component.name;
        _component.size = __component.size;

        settings.push_back(_component);
    }

    configured = true;
}

vector<struct component> config::components() {
    
    load();
    configure();

    return settings;
}

vector<size_t> config::configurations(const component& __component) {

    vector<size_t>  _configurations;

    for (auto ___component : settings)
        if (___component.name == __component.name)
            for (auto configuration : ___component.configurations)
                _configurations.push_back(hash<string>{}(configuration));

    if (_configurations.size() == 0)
        throw invalid_argument("no configuration for " + __component.name + " component found");

    return _configurations;
}

void config::add_component(component __component) {

    for (auto ___component : settings)
        if (___component.name == __component.name)
            throw new logic_error("component " + __component.name + " bad name. Multiple components within different configurations cannot have same name");
    
    settings.push_back(__component);
}

string config::get_configuration(const component& __component, size_t _configuration) {
    for (auto ___component : settings)
        if (___component.name == __component.name)
            for (auto __configuration : ___component.configurations)
                if (hash<string>{}(__configuration) == _configuration)
                    return __configuration;

    throw invalid_argument("either there is no component with name " + __component.name + ", or the component does not havy any configuration " + to_string(_configuration));
}

struct component config::get_component(string name) {

    for (auto _component : settings)
        if (_component.name == name)
            return _component;
    
    throw invalid_argument("no component with name " + name + " found");
}

int config::get_size(string name) {

    for (auto _component : settings)
        if (_component.name == name)
            return _component.size;
    
    throw invalid_argument("no component with name " + name + " found");
}

size_t config::add_configuration(const component &__component, const vector<string> &_configurations) {

    int                 i;

    string              configuration;

    for (auto &___component : settings)
        if (__component.name == ___component.name) {

            if (!___component.configurations.empty() && _configurations.size() + 1 != ___component.size)
                throw logic_error("component " + __component.name + " bad configuration size. Expected " + to_string(___component.size) + " but found " + to_string(_configurations.size() + 1));
            

            ___component.size = _configurations.size() + 1;

            configuration = "";

            for (auto _param : _configurations)
                configuration += " " + _param;

            configuration = configuration.erase(0, 1);

            for (auto _configuration : ___component.configurations)
                if (_configuration == configuration)
                    throw invalid_argument("configration " + configuration + " is duplicated.");

            ___component.configurations.push_back(configuration);

            return hash<string>{}(configuration);
        }

    settings.push_back(__component);
    return add_configuration(__component, _configurations);
}

void config::nested_combinations(struct _component ___component, string result_nested, vector<string>& combinations, int i, int shift, int last) {
    
    if ((___component.range_arguments.at(2 + shift) > 0 
            && i <= ___component.range_arguments.at(1 + shift)) ||
        (___component.range_arguments.at(2 + shift) <  0 
            && i <= ___component.range_arguments.at(1 + shift)
       )) {
        if (shift + 3 < last)
            nested_combinations(___component, 
                                result_nested + " " + to_string(i), 
                                combinations, 
                                ___component.range_arguments.at(3 + shift), 
                                3 + shift, 
                                last
            );
        else
            combinations.push_back(result_nested + " " + to_string(i));

        nested_combinations(___component, 
                            result_nested, 
                            combinations,
                            ___component.range_arguments.at(2 + shift) > 0 ?
                                abs(i) + ___component.range_arguments.at(2 + shift) 
                            :
                                i == 0  ?
                                    (-1) * ___component.range_arguments.at(2 + shift)
                                :
                                    (-1) * abs(i) * ___component.range_arguments.at(2 + shift),
                            shift, 
                            last
        );
    }        
    
}

void config::read_format_line(std::ifstream& file, string &line, int &line_number) {
  
    do {
        // erasing white spaces and iterating on comments

        getline(file, line); 
        ++line_number;
        line = utility_trim(line);
    } while ((line[0] == '#' || line.size() == 0) && file.peek() != EOF);

    // erasing eventual trailing comments

    line = line.substr(0, line.find("#", 0));   
}

bool config::trim_compare(const string& _left, const string& _right) {
   
    return utility_trim(_left).compare(_right) == 0;
}
