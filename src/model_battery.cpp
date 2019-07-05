
#include "../include/model_battery.hpp"

using namespace plnr;

model_battery::model_battery(config* _config, pathn* __model_1layer, first_derivative* __first_derivative) {
    _model_1layer = __model_1layer;
    _first_derivative = __first_derivative;
    h = _config->get_h();
}

model_battery::model_battery(pathn* __model_1layer, first_derivative* __first_derivative, double _h) {
    _model_1layer = __model_1layer;
    _first_derivative = __first_derivative;
    h = _h;
}

model_battery::~model_battery() { }

pathn* model_battery::get_model() {
    double          t =                 1.0,
                    t0 =                0.0;

    vectorn         start_y,
                    start_dy;

    vectorn*        y;
    vectorn*        dy;

    pathn*          _model_battery;
    integrator_rk4* _integrator_rk4;

    start_y = vectorn(_model_1layer->rows());
    start_y.inherit_flags(_model_1layer->get(0));
    start_y = start_y + 100.0;
    start_dy = _first_derivative->get_value(t0, start_y);

    _model_battery = new pathn(start_y);

    y = new vectorn(start_y.length());
    dy = new vectorn(start_dy.length());

    _integrator_rk4 = new integrator_rk4(_first_derivative, t0, start_y, h, start_dy);

    while (t < _model_1layer->columns() - 1) {
        _integrator_rk4->step(&t, y, dy);
        _model_battery->add(*y);
    }

    delete y;
    delete dy;
    delete _integrator_rk4;

    return _model_battery;
}
