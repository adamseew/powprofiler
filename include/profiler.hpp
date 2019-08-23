
#include <mutex>

#include "pathn.hpp"
#include "sampler.hpp"
#include "config.hpp"

#ifndef PLNR_PROFILER_H
#define PLNR_PROFILER_H

namespace plnr
{
    class profiler {
    
    private:
        bool        started = false;

        int         frequency;

        sampler*    _sampler;

        pathn*      _profile;

        std::mutex  started_mutex;

    public:
        profiler(config* _config, sampler* __sampler);

        profiler(int _frequency, sampler* __sampler);
        
        ~profiler();

        pathn* profile(const std::string& component, int milliseconds);

        pathn* profile(const std::string& component);

        pathn* profile();
    };
}

#endif
