
#include <tuple>

#include "integrator_rk4.hpp"
#include "model_1layer.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer {

    private:
        profiler*                                                   _profiler;
        config*                                                     _config;
        std::vector<std::pair<std::string, pathn*> >                _models;

        // used to store models 1layer when powprof is used in the "library version", i.e., the models 1layer are stored as they are profiled, and a model 2layer in generated later
        // first is the component name, then configuration, then model 1layer

        std::vector<std::tuple<std::string, std::string, pathn*> >  stored_model_1layer;

    public:
        model_2layer(config* __config, profiler* __profiler);

        model_2layer(std::vector<std::pair<std::string, pathn*> > __models);

        template <typename... params>
        model_2layer(const params&... _params);

        ~model_2layer();

        // used to add models 1layer (you know that this modality is selected when profiler is nullptr)

        model_2layer(config* __config);

        // you pass pair<size_t,pathn*> as variadic here, size_t is the configuration id (returned when adding a) and pathn* a pointer to the generated model 1layer

        template <typename... params>
        void add_model(const struct component &_component, const params&... _params);

        std::vector<std::pair<std::string, pathn*> > models();
    };
}

#endif
