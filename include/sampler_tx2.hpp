
#include "sampler.hpp"

#ifndef PLNR_SAMPLER_TX2
#define PLNR_SAMPLER_TX2

#define SAMPLES_COUNT       3
#define BUFFER_SIZE         31

namespace plnr 
{
    class sampler_tx2 : public sampler {

    private:
        int file_descriptors[SAMPLES_COUNT];
        
    public:
        sampler_tx2();

        ~sampler_tx2();

        virtual vectorn get_sample();

        virtual bool dryrun();
    };
}

#endif
