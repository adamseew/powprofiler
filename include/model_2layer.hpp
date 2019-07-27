
#include "integrator_rk4.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer {

    private:

    public:
        model_2layer(config* _config, first_derivative* _first_derivative);

        ~model_2layer();
    };
}

#endif
