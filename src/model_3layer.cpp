
#include "../include/model_3layer.hpp"

using namespace plnr;

model_3layer::model_3layer(pathn* __model_1layer, first_derivative* __first_derivative, double _h) {
    _model_1layer = __model_1layer;
    _first_derivative = __first_derivative;
    h = _h;
}

model_3layer::~model_3layer() {
    delete _model_1layer;
    delete _first_derivative;
}

pathn* model_3layer::get_model() {
    double          t =                 1.0,
                    t0 =                0.0;

    vectorn         start_y,
                    start_dy;

    vectorn*        y;
    vectorn*        dy;

    pathn*          _model_3layer;
    integrator_rk4* _integrator_rk4;

    start_y = *new vectorn(_model_1layer->rows(), new double[_model_1layer->rows()] {0.0});
    start_y.inherit_flags(_model_1layer->get(0));
    start_y = start_y + 100.0;
    start_dy = _first_derivative->get_value(t0, start_y);

    _model_3layer = new pathn(start_y);

    y = new vectorn(start_y.length());
    dy = new vectorn(start_dy.length());

    _integrator_rk4 = new integrator_rk4(_first_derivative, t0, start_y, h, start_dy);

    while (t < _model_1layer->columns() - 1) {
        _integrator_rk4->step(&t, y, dy);
        _model_3layer->add(*y);
    }


    delete y;
    delete dy;
    delete _integrator_rk4;

    return _model_3layer;
}
