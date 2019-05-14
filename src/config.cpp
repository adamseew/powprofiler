
#include "../include/utility.hpp"
#include "../include/config.hpp"

#include <fstream>

using namespace plnr;

using std::vector;
using std::string;
using std::runtime_error;
using std::to_string;
using std::exception;

config::config(const string& _file) {
    frequency = 10;
    h = 0.01;
    directory = "profiles";
    file = _file;
}

config::~config() { 
    vector<struct component>().swap(settings);
}

void config::load() {
    std::ifstream   input_cfg(file);

    int                 line_number =       0,
                        components_count =  0;

    string              line;
    vector<string>      property_value;

    struct component    _component;

    read_format_line(input_cfg, line, line_number);

    // first line should be [settings]

    if (utility_trim(line) != "[settings]")
        throw runtime_error("configuration file bad format line " + to_string(line_number) + ". File must start with [settings]");

    for ( ; ; ) {

        read_format_line(input_cfg, line, line_number);

        if (input_cfg.peek() == EOF)
            break;

        line = utility_trim(line);
        if (trim_compare(line, "[components]"))
            // line containing different components configuration ([components]) reached

            break;
        
        // expecting properties
        
        property_value = utility_split(line, '=');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

        property_value.at(1) = utility_trim(property_value.at(1));

        if (trim_compare(property_value.at(0), "frequency"))
            frequency = stoi(property_value.at(1));

        else if (trim_compare(property_value.at(0), "h"))
            h = stof(property_value.at(1));

        else if (trim_compare(property_value.at(0), "directory"))
            directory = property_value.at(1);
    }

    read_format_line(input_cfg, line, line_number);

    if (line[0] != '[')
        throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected a component description but found " + line);

    for ( ; ; ) {
        // expecting new component

        property_value = utility_split(line, '.');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected [component.name] but found " + line);

        _component.name = utility_trim(property_value.at(1));
        _component.name = _component.name.substr(0, _component.name.size() - 1);

        read_format_line(input_cfg, line, line_number);

        // expecting component source

        property_value = utility_split(line, '=');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

        _component.src = utility_trim(property_value.at(1));
        _component.arguments.clear();

        // expecting a number of possible arguments. min,max,step for each argument         

        for (read_format_line(input_cfg, line, line_number);
             line[0] != '[' && !input_cfg.eof();
             read_format_line(input_cfg, line, line_number)
            ) {

            property_value = utility_split(line, '=');

            if (property_value.size() != 2)
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

            property_value = utility_split(property_value.at(1), ',');

            if (property_value.size() != 3)
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value,value,value but found " + line);

            _component.arguments.push_back(stoi(utility_trim(property_value.at(0))));
            _component.arguments.push_back(stoi(utility_trim(property_value.at(1))));
                
            try {
                property_value.at(2) = utility_trim(property_value.at(2));

                if (property_value.at(2)[0] == 'p') {
                    property_value = utility_split(property_value.at(2), '(');

                    // if the line start by p, it means that a pow operator is involed. To save this operator into the data struct, the integer is stored as negative (just for convenience!)

                    _component.arguments.push_back(
                        (-1) * stoi(property_value.at(1).substr(0, property_value.at(1).size()))
                    );
                } else
                    _component.arguments.push_back(stoi(property_value.at(2)));

            } catch (exception &_exception) {
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value or pow(value) but found " + line);
            }
        }

        settings.push_back(_component);
        ++components_count;

        if (input_cfg.peek() == EOF)
            break;
    }
    
    if (components_count < 1)
        throw runtime_error("configuration file bad format line " + to_string(line_number) + ". At least one component expected");

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
