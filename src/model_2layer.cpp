
#include "../include/model_battery.hpp"
#include "../include/soc_1resistor.hpp"
#include "../include/model_2layer.hpp"
#include "../include/utility.hpp"

using namespace plnr;

using std::vector;
using std::pair;
using std::exception;

model_2layer::model_2layer(config* __config, profiler* __profiler) {
    _config = __config;
    _profiler = __profiler;
}

model_2layer::model_2layer(vector<pair<struct component, pathn*> > __models, profiler* __profiler) {
    _config = NULL;
    _profiler = __profiler;
    _models = __models;
}

model_2layer::~model_2layer() {
     vector<pair<struct component, pathn*> >().swap(_models);
}

vector<pair<struct component, pathn*> > model_2layer::models() {
    
    int                             i,
                                    j,
                                    merged_size = 0;

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

    // todo: to debug, properly

    for (auto component : _config->components()) {
        
        _model = new pathn();

        for (auto configuration : component.configurations) {

            __model_1layer = new model_1layer(configuration, _profiler);
            _model_1layer = __model_1layer->get_model();

            delete __model_1layer;

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
                        ___flags.empty();
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

                merged = new vectorn(merged_size, _flags);

                merged_initialized = true;
            }

            i = 0;
            for (auto _configuration : utility_split(configuration, ' '))
                if (utility_is_number(_configuration)) {
                    configuration_value = stod(configuration);
                    merged->set(i++, configuration_value);
                }

            for (j = 0; j < power_1layer.length(); j++)
                merged->set(i + j, power_1layer.get(i));

            energy_1layer = _model_1layer->sum();

            for (j = 0; j < energy_1layer.length(); j++)
                merged->set(
                    i + j + power_1layer.length(), 
                    energy_1layer.get(i));

            _first_derivative = new soc_1resistor(*_model_1layer / 12.0, 14.8, 0.0012, 12, 5);

            __model_battery = new model_battery(_config, _model_1layer, _first_derivative);
            _model_battery = __model_battery->get_model();

            delete _model_1layer;
            delete _first_derivative;
            delete __model_battery;

            soc = _model_battery->get(_model_battery->columns() - 1);

            for (j = 0; i < soc.length(); i++)
                merged->set(
                    i + j + power_1layer.length() + energy_1layer.length(), 
                    soc.get(i));
                
            delete _model_battery;

            _model->add(*merged);
        }

        delete merged;
        merged_initialized = true;

        _models.push_back(pair<struct component, pathn*>(component, _model));
    }
}
