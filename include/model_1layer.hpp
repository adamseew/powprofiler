
#include "profiler.hpp"
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
        std::string     component;
        std::string     arguments;
        profiler*       _profiler;
        pathn*          _model;

    public:
        model_1layer(std::string _component, profiler* __profiler);

        model_1layer(std::string _component, std::string _arguments, profiler* __profiler);

        model_1layer(pathn* __model, profiler* __profiler);

        ~model_1layer();

        virtual pathn* get_model();
    };
}

#endif
