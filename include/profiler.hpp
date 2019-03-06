
#include "pathn.hpp"
#include "sampler.hpp"

#ifndef PLNR_PROFILER_H
#define PLNR_PROFILER_H

namespace plnr
{
    class profiler {
    
    private:
        int         frequency;
        sampler*    _sampler;

    public:
        profiler(int _frequency, sampler* __sampler);

        pathn profile(std::string component, int milliseconds);

        pathn profile(std::string component);
    };
}

#endif
