
#include "../include/model_battery.hpp"
#include "../include/soc_1resistor.hpp"
#include "../include/model_2layer.hpp"
#include "../include/utility.hpp"

#include <functional>
#include <stdexcept>

using namespace plnr;

using std::invalid_argument;
using std::make_tuple;
using std::exception;
using std::to_string;
using std::string;
using std::vector;
using std::tuple;
using std::pair;
using std::get;

model_2layer::model_2layer(config* __config, profiler* __profiler) {
    _config = __config;
    _profiler = __profiler;
}

model_2layer::model_2layer(config* __config) {
    _config = __config;
    _profiler = nullptr;
}

template <typename... params>
model_2layer::model_2layer(const params&... _params) {

    vector<pair<string, pathn*> >    __params = {_params...};

    for (auto _param : __params) {

        // so any only since cpp 17 ... no strong typization though
        
        //if (__params.at(i).get_type() != any::pair<struct component, pathn*>)
        //    throw invalid_argument(
        //        "bad argument (index " + to_string(i) + "). Expected pair<struct component, pathn*> but found " + __params.at(i).name()
        //    );
        
        _models.push_back(_param);
    }
}

model_2layer::model_2layer(vector<pair<string, pathn*> > __models) {
    _config = NULL;
    _profiler = nullptr;
    _models = __models;
}

model_2layer::~model_2layer() {
     vector<pair<string, pathn*> >().swap(_models);
     vector<tuple<string, string, pathn*> >().swap(stored_model_1layer);
}

template <typename... params>
void model_2layer::add_model(const struct component &_component, const params&... _params) {

    string                      _configuration = "";
    
    vector<pair<size_t, pathn*> >  __params =  {_params...};

    for (auto _param : __params) {
        for (auto __configuration : _component.configurations)
            if (std::hash<string>{}(__configuration) == _param.first)
                _configuration = __configuration;

        if (_configuration.empty())
            throw invalid_argument("Configuration (id " + to_string(_param.first) + ") is invalid");

        stored_model_1layer.push_back(make_tuple(_component.name, _configuration, _param.second));
    }
}

vector<pair<string, pathn*> > model_2layer::models() {
    
    int                             i,
                                    j;

    bool                            merged_initialized = false;

    double                          configuration_value;

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
    pathn*                          _model;

    if (!_models.empty())
        return _models;

    for (auto component : _config->components()) {
        
        _model = new pathn();

        for (auto configuration : component.configurations) {
            
            if (_profiler) {

                __model_1layer = new model_1layer(configuration, _profiler);

                _model_1layer = __model_1layer->get_model();

                delete __model_1layer;
            } else {
                i = 0;
                for (auto _stored_model_1layer : stored_model_1layer) {
                    
                    i++;

                    if (static_cast<string>(get<0>(_stored_model_1layer)) == component.name &&
                        static_cast<string>(get<1>(_stored_model_1layer)) == configuration
                    ) {
                        _model_1layer = static_cast<pathn*>(get<2>(_stored_model_1layer));
                        stored_model_1layer.erase(stored_model_1layer.begin() + i);
                    }
                }
            }

            power_1layer = _model_1layer->avg();

            /// initializing vector that will contain data saved later into model

            if (!merged_initialized) {
                
                // columns for configurations
                i = 0;
                for (auto _configuration : utility_split(configuration, ' ')) {
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
            for (auto _configuration : utility_split(configuration, ' '))
                if (utility_is_number(_configuration)) {
                    configuration_value = stod(_configuration);
                    merged->set(i++, configuration_value);
                }

            for (j = 0; j < power_1layer.length(); j++)
                merged->set(i + j, power_1layer.get(j));

            energy_1layer = _model_1layer->sum();

            for (j = 0; j < energy_1layer.length(); j++)
                merged->set(
                    i + j + power_1layer.length(), 
                    energy_1layer.get(j));

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

        _models.emplace_back(component.name, _model);
    }

    return _models;
}
