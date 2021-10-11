
#include "../include/model_battery.hpp"
#include "../include/soc_1resistor.hpp"
#include "../include/model_1layer.hpp"
#include "../include/model_2layer.hpp"
#include "../include/utility.hpp"

#include <functional>
#include <stdexcept>

using namespace plnr;

using std::invalid_argument;
using std::runtime_error;
using std::make_tuple;
using std::exception;
using std::to_string;
using std::string;
using std::vector;
using std::tuple;
using std::pair;
using std::get;

model_2layer::model_2layer(config* __config, profiler* __profiler, const component& __component) {

    _component = __component;
    _config = __config;
    _profiler = __profiler;
    _model = nullptr;
}

model_2layer::model_2layer(config* __config, const component& __component) {

    _component = __component;
    _config = __config;
    _profiler = nullptr;
    _model = nullptr;
}

model_2layer::model_2layer(pathn* __model) {

    _config = nullptr;
    _profiler = nullptr;
    _model = __model;
}

model_2layer::~model_2layer() { }

pathn* model_2layer::get_model() {
    
    int                             i,
                                    j;

    bool                            merged_initialized = false;

    double                          configuration_value;

    string                          configuration_string;

    vectorn                         energy_1layer,
                                    power_1layer,
                                    working_copy,
                                    soc;

    vectorn*                        merged;

    vector<vectorn_flags>           ___flags;

    vector<vector<vectorn_flags> >  __flags,
                                    _flags;

    first_derivative*               _first_derivative;

    model_battery*                  __model_battery;

    model_1layer*                   __model_1layer;

    pathn*                          _model_1layer;
    pathn*                          _model_battery;

    if (_model != nullptr)
        return _model;
        
    _model = new pathn();

    for (auto _configuration : _config->configurations(_component.name)) {
            
        configuration_string = _config->get_configuration(_component, _configuration);    

        if (!_component.src.empty()) {

            __model_1layer = new model_1layer(_config, _profiler, _component, _configuration);

            _model_1layer = __model_1layer->get_model();

            delete __model_1layer;
        } else {

            for (auto stored_model : stored_models)
                if (_configuration == stored_model.first)
                    _model_1layer = stored_model.second;

            if (_model_1layer)
                continue;
        }

        power_1layer = _model_1layer->avg();

        /// initializing vector that will contain data saved later into model

        if (!merged_initialized) {
                
            // columns for configurations
            i = 0;
            for (auto _configuration : utility_split(configuration_string, ' ')) {
                if (utility_is_number(_configuration)) {

                    ___flags.push_back(static_cast<vectorn_flags>(
                        static_cast<int>(vectorn_flags::problem_dimension) + i++
                    ));
                        
                    _flags.push_back(___flags);
                    ___flags.clear();
                }
            }

            // columns for power

            working_copy = power_1layer;
            __flags = working_copy.get_flags();
            _flags.insert(_flags.end(), __flags.begin(), __flags.end());
                
            // columns for energy

            working_copy.transform_flags(vectorn_flags::gain);
            __flags = working_copy.get_flags();
            _flags.insert(_flags.end(), __flags.begin(), __flags.end());

            // columns for soc
            
            working_copy.transform_flags(vectorn_flags::gain);
            __flags = working_copy.get_flags();
            _flags.insert(_flags.end(), __flags.begin(), __flags.end());

            merged = new vectorn(_flags.size(), _flags);

            merged_initialized = true;
        }

        i = 0;
        for (auto _configuration : utility_split(configuration_string, ' '))
            if (utility_is_number(_configuration)) {
                configuration_value = stod(_configuration);
                merged->set(i++, configuration_value);
            }

        for (j = 0; j < power_1layer.length(); j++)
            merged->set(i + j, power_1layer.get(j));

        energy_1layer = _model_1layer->sum()/_config->get_frequency();

        for (j = 0; j < energy_1layer.length(); j++)
            merged->set(
                i + j + power_1layer.length(), 
                energy_1layer.get(j));


        /// here you can change values of the constant of the battery model!

        _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

        __model_battery = new model_battery(_config, _model_1layer, _first_derivative);
        _model_battery = __model_battery->get_model();

        delete _model_1layer;
        delete _first_derivative;
        delete __model_battery;

        soc = _model_battery->get(_model_battery->columns() - 1);

        for (j = 0; j < soc.length(); j++)
            merged->set(
                i + j + power_1layer.length() + energy_1layer.length(),
                soc.get(j));
                
        delete _model_battery;

        _model->add(*merged);
    }

    delete merged;
    merged_initialized = false;

    return _model;
}

void model_2layer::add_model(size_t _configuration, pathn* _model) {
    
    for (auto stored_model : stored_models)
        if (stored_model.first == _configuration)
            throw runtime_error("the configuration was added before");

    // just checking if the configuration is stored in the config and saving it
    
    _config->get_configuration(_component, _configuration);

    stored_models.emplace_back(_configuration, _model);
}

void model_2layer::add_model(vector<pathn*> _models) {

    int             i;
    
    vector<size_t>  configurations_per_component;

    if (configurations_per_component.size() != _models.size())
        throw invalid_argument("models size does not equal to the number of configuration of " + _component.name + " component. Pass " + to_string(configurations_per_component.size()) + " models");

    for (i = 0; i < _models.size(); i++)
        add_model(configurations_per_component.at(i), _models.at(i));
}
