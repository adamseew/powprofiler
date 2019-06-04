
#include "integrator_rk4.hpp"
#include "model_1layer.hpp"
#include "pathn.hpp"

#ifndef PLNR_MODEL_3LAYER_H
#define PLNR_MODEL_3LAYER_H

namespace plnr
{
    class model_battery : public model {

    private:
        double              h;
    
        pathn*              _model_1layer;
        first_derivative*   _first_derivative;

    public:
        model_battery(config* _config, pathn* __model_1layer, first_derivative* __first_derivative);

        model_battery(pathn* __model_1layer, first_derivative* __first_derivative, double _h);

        ~model_battery();

        virtual pathn* get_model();
    };
}

#endif
