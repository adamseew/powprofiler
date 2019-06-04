
#include "pathn.hpp"
#include "sampler.hpp"
#include "config.hpp"

#ifndef PLNR_PROFILER_H
#define PLNR_PROFILER_H

namespace plnr
{
    class profiler {
    
    private:
        int         frequency;
        sampler*    _sampler;

    public:
        profiler(config* _config, sampler* __sampler);

        profiler(int _frequency, sampler* __sampler);
        
        ~profiler();

        pathn profile(std::string component, int milliseconds);

        pathn profile(std::string component);
    };
}

#endif
