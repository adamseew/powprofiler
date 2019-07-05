
#include "../include/model_1layer.hpp"

using namespace plnr;

model_1layer::model_1layer(std::string _component, profiler* __profiler) {
    component = _component;
    arguments = "";
    _profiler = __profiler;
    _model = NULL;
}

model_1layer::model_1layer(std::string _component, std::string _arguments, profiler* __profiler) {
    component = _component;
    arguments = _arguments;
    _profiler = __profiler;
    _model = NULL;
}

model_1layer::model_1layer(pathn* __model, profiler* __profiler) {
    component = "";
    arguments = "";
    _profiler = __profiler;
    _model = __model;
}

model_1layer::~model_1layer() { }

pathn* model_1layer::get_model() {

    if (_model == NULL) {
        // todo

        // the profiler profiles a specific component until the component has not terminated its execution. If we want to terminate in a specific amount of ms just call ...er->profile(component, ms)...

        if (arguments.empty())
            _model = new pathn(_profiler->profile(component));
        else
            _model = new pathn(_profiler->profile(component + " " + arguments));
    }

    return _model;
}


