
#include <tuple>

#include "integrator_rk4.hpp"
#include "model_1layer.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer : public model  {

    private:
        struct component    _component;

        profiler*           _profiler;

        config*             _config;

        pathn*              _model;

        // todo: what was this?

        // used to store models 1layer when powprof is used in the "library version", i.e., the models 1layer are stored as they are profiled, and a model 2layer in generated later
        // first is the component name, then configuration, then model 1layer

        // std::vector<std::tuple<std::string, std::string, pathn*> >  stored_model_1layer;

    public:
        model_2layer(config* __config, profiler* __profiler, const component& __component);

        model_2layer(pathn* __model);

        ~model_2layer();

        virtual pathn* get_model();
    };
}

#endif
