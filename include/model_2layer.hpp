
#include <tuple>

#include "integrator_rk4.hpp"
#include "model_1layer.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer {

    private:
        profiler*                                           _profiler;
        config*                                             _config;
        std::vector<std::pair<std::string, pathn*> >        _models;

        // used to store models 1layer when powprof is used in the "library version", i.e., the models 1layer are stored as they are profiled, and a model 2layer in generated later

        std::vector<std::tuple<std::string, int, pathn*> >  __models;

    public:
        model_2layer(config* __config, profiler* __profiler);

        model_2layer(std::vector<std::pair<std::string, pathn*> > ___models);

        template <typename... params>
        model_2layer(const params&... _params);

        ~model_2layer();

        // used to add models 1layer (you know that this modality is selected when profiler is nullptr)

        model_2layer(config* __config);

        // you pass pair<int,pathn*> as variadic here, int is the configuration id and pathn* a pointer to the generated model 1layer

        template <typename... params>
        void add_model(const struct component &_component, const params&... _params);

        std::vector<std::pair<std::string, pathn*> > models();
    };
}

#endif
