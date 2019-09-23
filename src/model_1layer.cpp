
#include "../include/model_1layer.hpp"

#include <thread>

using namespace plnr;

using std::string;
using std::thread;

model_1layer::model_1layer(profiler* __profiler) {

    _config = nullptr;
    _profiler = __profiler;
    _model = nullptr;
    started = false;
    stopped = false;
}

model_1layer::model_1layer(config* __config, profiler* __profiler, const component& __component, size_t __configuration) {

    _config = __config;
    _profiler = __profiler;

    _component = __component;
    _configuration = __configuration;

    _model = nullptr;
    started = false;
    stopped = false;
}

model_1layer::model_1layer(pathn* __model) {

    _config = nullptr;
    _profiler = nullptr;
    _model = __model;
    started = false;
    stopped = false;
}

model_1layer::~model_1layer() { }

pathn* model_1layer::get_model() {

    if (_model == nullptr) {
        
        if (_config == nullptr)
            throw std::runtime_error("Model not generated. Call start stop first, or use a valid config");

        
        _model = _profiler->profile(
            _config->get_configuration(_component, _configuration),
            _component.runtime
        );
    }

    return _model;
}

void model_1layer::start() {

    _profiler->profile();
}

void model_1layer::stop() {

    _model = _profiler->profile();
}
