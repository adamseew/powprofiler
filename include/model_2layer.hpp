
#include <tuple>

#include "integrator_rk4.hpp"
#include "model_1layer.hpp"

#ifndef PLNR_MODEL_2LAYER_H
#define PLNR_MODEL_2LAYER_H

namespace plnr
{
    class model_2layer : public model  {

    private:
        struct component                        _component;

        profiler*                               _profiler;

        config*                                 _config;

        pathn*                                  _model;

        // used to store models 1layer when powprof is used in the "library version", i.e., the models 1layer are stored as they are profiled, and a model 2layer in generated later

        std::vector<std::pair<size_t, pathn*> > stored_models; // first configuration, then model 1layer

    public:
        model_2layer(config* __config, profiler* __profiler, const component& __component);

        model_2layer(config* __config, const component& __component);

        model_2layer(pathn* __model);

        ~model_2layer();

        virtual pathn* get_model();

        void add_model(size_t _configuration, pathn* _model);

        // following function expects to receive as input a number of models that automatically matches to the configurations of the component. The developer must ensure that: (1) number of models corresponds to the number of configurations (otherwise exception), (2) that the models are added in the same order as the configurations are (otherwise no exception but unpredictable behavior)

        void add_model(std::vector<pathn*> _models);

        // variadic overload of the above function, for commodity (so it can be invoked with a variable number of parameters)

        template<typename... params>
        void add_model(pathn* _param, params... _params)  {

            std::vector<pathn*>     variadic_vector;

            int                     dummy[] = { 0, (variadic_vector.push_back(_params), 0)... };
            (void) dummy;
            variadic_vector.insert(variadic_vector.begin(), _param);

            add_model(variadic_vector);
        }
    };
}

#endif
