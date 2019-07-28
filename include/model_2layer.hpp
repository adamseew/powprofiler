
#include "integrator_rk4.hpp"
#include "model_1layer.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer {

    private:
        first_derivative*                                   _first_derivative;
        profiler*                                           _profiler;
        config*                                             _config;
        std::vector<std::pair<struct component, pathn*> >   _models;

    public:
        model_2layer(config* __config, first_derivative* __first_derivative, profiler* __profiler);

        model_2layer(std::vector<std::pair<struct component, pathn*> > __models, profiler* __profiler);

        ~model_2layer();

        std::vector<std::pair<struct component, pathn*> > models();
    };
}

#endif
