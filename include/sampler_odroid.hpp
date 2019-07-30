
#include "sampler.hpp"

#ifndef PLNR_SAMPLER_ODROID
#define PLNR_SAMPLER_ODROID

#define SAMPLES_COUNT       3
#define BUFFER_SIZE         31

namespace plnr 
{
    class sampler_odroid : public sampler {

    private:
        int file_descriptors[SAMPLES_COUNT];
        
    public:
        sampler_odroid();

        ~sampler_odroid();

        virtual vectorn get_sample();

        virtual bool dryrun();
    };
}

#endif
