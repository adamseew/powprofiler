
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

    public:
        model_1layer();

        ~model_1layer();

        virtual pathn* get_model();
    };
}

#endif
