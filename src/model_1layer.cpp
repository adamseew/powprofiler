
#include "../include/model_1layer.hpp"

using namespace plnr;

using std::string;

model_1layer::model_1layer(string _component, profiler* __profiler) {
    component = _component;
    arguments = "";
    _profiler = __profiler;
    _model = nullptr;
    started = false;
    stopped = false;
}

model_1layer::model_1layer(string _component, std::string _arguments, profiler* __profiler) {
    component = _component;
    arguments = _arguments;
    _profiler = __profiler;
    _model = nullptr;
    started = false;
    stopped = false;
}

model_1layer::model_1layer(pathn* __model) {
    component = "";
    arguments = "";
    _profiler = nullptr;
    _model = __model;
    started = false;
    stopped = false;
}

model_1layer::~model_1layer() { }

pathn* model_1layer::get_model() {

    if (_model == NULL) {
        
        // todo

        // the profiler profiles a specific component until the component has not terminated its execution. If we want to terminate in a specific amount of ms just call ...er->profile(component, ms)...

        if (arguments.empty())
            _model = _profiler->profile(component);
        else
            _model = _profiler->profile(component + " " + arguments);
    }

    return _model;
}

void model_1layer::start() {

    _profiler->profile();
}

void model_1layer::stop() {

    _model = _profiler->profile();
}
