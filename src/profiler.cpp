
#include "../include/profiler.hpp"

#include <thread>

using namespace plnr;
using namespace std;

profiler::profiler(int _frequency, sampler* __sampler) {
    frequency = _frequency;
    _sampler = __sampler;
}

pathn profiler::profile(std::string component, int milliseconds) {
    pathn                   _profile;

    chrono::milliseconds    timespan(1000 / frequency);
    chrono::milliseconds    max_sampling_time(milliseconds);

    std::thread             sampler_thread([&](){ 

        _profile.add(_sampler->get_sample()); 
        this_thread::sleep_for(timespan);
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