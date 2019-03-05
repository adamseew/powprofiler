
#include "sampler.hpp"

#ifndef PLNR_SAMPLER_TX2
#define PLNR_SAMPLER_TX2

#define SAMPLES_COUNT       3
#define BUFFER_SIZE         31

namespace plnr 
{
    class sampler_tx2 : public sampler {
        
    public:
        ~sampler_tx2();

        virtual vectorn get_sample();
    };
}

#endif