
#include "../include/utility.hpp"
#include "../include/config.hpp"

#include <sys/stat.h>
#include <fstream>
#include <cmath> 

using namespace plnr;

using std::runtime_error;
using std::logic_error;
using std::to_string;
using std::exception;
using std::vector;
using std::string;
using std::pair;

config::config(const string& _file) {
    frequency = 10;
    h = 0.01;
    directory = "profiles";
    file = _file;
}

config::~config() { 
    vector<struct component>().swap(settings);
    vector< vector<string> >().swap(configurations);
}

double config::get_frequency() {
    return frequency;
}

double config::get_h() {
    return h;
}

void config::load() {
    if (!file_exists(file))
        throw logic_error("configuration file " + file + " does not exist");

    std::ifstream       input_cfg(file);

    int                 i,
                        line_number =       0,
                        components_count =  0,
                        min,
                        max,
                        step;
    bool                was_previous_range;

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

        _component.name = utility_trim(property_value.at(1));
        _component.name = _component.name.substr(0, _component.name.size() - 1);

        read_format_line(input_cfg, line, line_number);

        // expecting component source

        property_value = utility_split(line, '=');

        if (property_value.size() != 2)
            throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

        _component.src = utility_trim(property_value.at(1));

        if (!file_exists(_component.src))
            throw logic_error("configuration file error line " + to_string(line_number) + ". Source file " + _component.src + " does not exist");

        _component.fixed_arguments.clear();
        _component.range_arguments.clear();
        _component.positions.clear();
        was_previous_range = false;

        // expecting a number of possible arguments. min,max,step for each argument         

        for (read_format_line(input_cfg, line, line_number);
             line[0] != '[';
             read_format_line(input_cfg, line, line_number)
            ) {

            property_value = utility_split(line, '=');

            if (property_value.size() != 2)
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected property=value but found " + line);

            if (trim_compare(property_value.at(0), "range")) {
                
                if (was_previous_range)
                    _component.positions.at(_component.positions.size() - 1).first = _component.range_arguments.size() + 3;
                else
                    _component.positions.emplace_back(_component.range_arguments.size() + 3, _component.range_arguments.size());

                was_previous_range = true;

                property_value = utility_split(property_value.at(1), ',');

                if (property_value.size() != 3)
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value,value,value but found " + line);

                try {
                    min = stoi(utility_trim(property_value.at(0)));
                    max = stoi(utility_trim(property_value.at(1)));
                    _component.range_arguments.push_back(min);
                    _component.range_arguments.push_back(max);
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
                    
                    _component.range_arguments.push_back(step);

                } catch (exception &_exception) {
                    throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Expected value or pow(value) with integer value but found " + line);
                }
            } else if (trim_compare(property_value.at(0), "fixed")) {
                _component.positions.emplace_back(0, _component.fixed_arguments.size());
                was_previous_range = false;

                _component.fixed_arguments.push_back(utility_trim(property_value.at(1)));

            } else {
                throw runtime_error("configuration file bad format line " + to_string(line_number) + ". Property " + utility_trim(property_value.at(0)) + " not recognized");
            }
            if ( max < 0 || 
                 min < 0 ||
                (max - min) < 0 || 
                (step > 0 && (max - min) < step) || 
                (step < 0 && (max - min) < step * step
               )) {
                throw logic_error("configuration file error line " + to_string(line_number) + ". Invalid format for range argument. Use min,max,step pattern where min,max are positive integers, step can be a natural or a power expressed as pow(step)");
            }

            if (input_cfg.peek() == EOF)
                break;
        }

        settings.push_back(_component);
        ++components_count;

        if (input_cfg.peek() == EOF)
            break;
    }
    
    if (components_count < 1)
        throw logic_error("configuration file error line " + to_string(line_number) + ". At least one component expected");

    input_cfg.close();

}

void config::configure() {
    if (configurations.size() == 0) {
        int                     i,
                                j,
                                k;

        vector<string>          arguments_combinations,
                                component_configurations,
                                _component_configurations;
        
        vector<vector<string> > _configurations;

        for (auto _component : settings) {


            component_configurations.clear();
            component_configurations.push_back(_component.src);

            for (auto position : _component.positions) {
                if (position.first == 0)
                    for (i = 0; i < component_configurations.size(); i++)
                        component_configurations.at(i) += " " + _component.fixed_arguments.at(position.second);

                else {
                    arguments_combinations.clear();
                    nested_combinations(_component, "", arguments_combinations, _component.range_arguments.at(position.second), position.second, position.first); 
                    _component_configurations.clear();
                    for (i = 0; i < component_configurations.size(); i++) {
                        for (auto combination : arguments_combinations)
                            _component_configurations.push_back(component_configurations.at(i) + " " + combination);
                    }
                    if (_component_configurations.size() > component_configurations.size()) {
                        component_configurations.clear();
                        component_configurations.insert(component_configurations.end(), _component_configurations.begin(), _component_configurations.end());
                    }
                }
            }

            _configurations.push_back(component_configurations);
        }
        
        // todo
        //nested_configurations(_configurations, configurations, 0);

        //nested_configurations(_configurations, configurations, vector<string>(), 0, 0);

        for (i = 0; i < _configurations.at(0).size(); i++) {
            
            vector<string> ri;
            ri.push_back({_configurations.at(0).at(i)});

            for (j = 0; j < _configurations.at(1).size(); j++) {
                
                vector<string> rj(ri);
                rj.push_back({_configurations.at(1).at(j)});


                for (k = 0; k < _configurations.at(2).size(); k++) {

                    vector<string> rk(rj);
                    rk.push_back({_configurations.at(2).at(k)}); 
                    
                    configurations.push_back(vector<string>({
                        rk
                    }));
                }
            }
        }
        i = 0;
    }
}

// todo
void config::nested_configurations(vector<vector<string> > __configurations, vector<vector<string> >& _configurations, vector<string> result_nested, int i, int l) {

    if (i < __configurations.at(l).size()) {
        
        vector<string> rn(result_nested);
        rn.push_back({__configurations.at(l).at(i)});

        if (l + 1 < __configurations.size())
            nested_configurations(__configurations, _configurations, rn, i, l + 1);
        else
            configurations.push_back(vector<string>({rn}))
            ;
        
        //result_nested.clear();
        nested_configurations(__configurations, _configurations, rn, i + 1, l);
               
    }
    
    
}

void config::nested_combinations(struct component _component, string result_nested, vector<string>& combinations, int i, int shift, int last) {
    
    if ((_component.range_arguments.at(2 + shift) > 0 
            && i <= _component.range_arguments.at(1 + shift)) ||
        (_component.range_arguments.at(2 + shift) <  0 
            && i <= _component.range_arguments.at(1 + shift)
       )) {
        if (shift + 3 < last)
            nested_combinations(_component, 
                                result_nested + " " + to_string(i), 
                                combinations, 
                                _component.range_arguments.at(3 + shift), 
                                3 + shift, 
                                last
            );
        else
            combinations.push_back(result_nested + " " + to_string(i));

        nested_combinations(_component, 
                            result_nested, 
                            combinations,
                            _component.range_arguments.at(2 + shift) > 0 ?
                                abs(i) + _component.range_arguments.at(2 + shift) 
                            :
                                i == 0  ?
                                    (-1) * _component.range_arguments.at(2 + shift)
                                :
                                    (-1) * abs(i) * _component.range_arguments.at(2 + shift),
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


bool config::file_exists(const std::string& name) {

    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}
