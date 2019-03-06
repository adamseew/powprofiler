
#include "../include/profiler.hpp"

#include <thread>
#include <mutex> 

using namespace plnr;
using namespace std;

profiler::profiler(int _frequency, sampler* __sampler) {
    frequency = _frequency;
    _sampler = __sampler;
}

pathn profiler::profile(std::string component, int milliseconds) {
    
    mutex                   _mutex;

    chrono::milliseconds    timespan(1000 / frequency);
    chrono::milliseconds    max_sampling_time(milliseconds);
    
    pathn                   _profile;
    vectorn                 sample(_sampler->get_sample());

    std::thread             sampler_thread([&](){ 
        _mutex.lock();
        sample = (sample + _sampler->get_sample()) / 2;
        _mutex.unlock();
    });
    std::thread             profiler_thread([&](){ 
        while (1) {
            _mutex.lock();
            _profile.add(sample); 
            _mutex.unlock();
            this_thread::sleep_for(timespan);
        }
    });
    std::thread             benchmark_thread([&](){ system(component.c_str()); });
    
    if (milliseconds > 0) {
        std::thread         time_thread([&](){ this_thread::sleep_for(max_sampling_time); });
        time_thread.join();
    } else {
        benchmark_thread.join();
    }

    return _profile;
}

pathn profiler::profile(std::string component) {
    return profiler::profile(component, 0);
}