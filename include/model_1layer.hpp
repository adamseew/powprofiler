
#include "profiler.hpp"
#include "config.hpp"
#include "pathn.hpp"

#ifndef PLNR_MODEL_1LAYER_H
#define PLNR_MODEL_1LAYER_H

namespace plnr
{
    class model {
        
    public:
        virtual ~model() { };

        virtual pathn* get_model() = 0;
    };

    class model_1layer : public model {

    private:
        bool                started,
                            stopped;

        struct component    _component;
        size_t              _configuration;

        profiler*           _profiler;
        
        config*             _config;

        pathn*              _model;

    public:

        // library modality (i.e., somebody calls start and stop)

        model_1layer(profiler* __profiler);

        // regular call relative to a given component and configuration 

        model_1layer(config* __config, profiler* __profiler, const component& __component, size_t __configuration);

        model_1layer(pathn* __model);

        ~model_1layer();

        virtual pathn* get_model();

        void start();

        void stop();
    };
}

#endif
