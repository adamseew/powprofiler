
#include "sampler.hpp"

#ifndef PLNR_SAMPLER_NANO
#define PLNR_SAMPLER_NANO

#define SAMPLES_COUNT       3
#define BUFFER_SIZE         31

namespace plnr 
{
    class sampler_nano : public sampler {

    private:
        int file_descriptors[SAMPLES_COUNT];
        
    public:
        sampler_nano();

        ~sampler_nano();

        virtual vectorn get_sample();
    };
}

#endif
