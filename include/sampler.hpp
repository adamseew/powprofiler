
#include "vectorn.hpp"

#ifndef PLNR_SAMPLER_H
#define PLNR_SAMPLER_H

namespace plnr 
{
    class sampler {
    
    public:
        virtual ~sampler() { }

        virtual vectorn get_sample() = 0;
    };
}

#endif